
#include "renderer_model_man.h"
#include "renderer_model_man_ref.h"
#include "renderer_model_man_readlock.h"
#include "renderer_model_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "renderer_model_man_task.h"
#include "../../core/dptaskpool/dptaskpool_ref.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "../../core/dptaskpool/dptaskpool_readlock.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "../renderer.h"
#include "../renderer_ref.h"
#include "../renderer_writelock.h"
#include "../../gfx/gfx_writelock.h"
#include "../../gfx/gfx_ref.h"
#include "../../gfx/model/model_man_ref.h"
#include "../../gfx/model/model_man_readlock.h"
#include "renderer_model.h"
#include "renderer_model_writelock.h"
#include "renderer_model_readlock.h"
#include "../../gfx/model/model.h"
#include "../../gfx/model/model_readlock.h"
#include "../../gfx/model/model_writelock.h"
#include "../../core/dpbtree/dpid_btree.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man::renderer_model_man( core *c, renderer *r, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        renderer_writelock *rl;
        
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        
        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_model_man::renderer_model_man" );
        if( gl )
        {
            gl->getModels( &this->g_models );
        }
        o.unlock();
        
        rl = (renderer_writelock *)o.writeLock( r, "renderer_model_man::renderer_model_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();
        
        this->_startTask( tp, 30 );
    }
    
    //dtor
    renderer_model_man::~renderer_model_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->deleteModels();
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->_deleteTask();
        delete this->r;
        delete this->g;
        delete this->g_models;
        delete this->tpr;
        o.unlock();
    }
    
    //return core
    core *renderer_model_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *renderer_model_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_model_man_readlock( (renderer_model_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_model_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_model_man_writelock( (renderer_model_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_model_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_model_man_ref( (renderer_model_man *)p, k );
    }
    
    //start task
    void renderer_model_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new renderer_model_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "renderer_model_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void renderer_model_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "renderer_model_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void renderer_model_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //sync models
    void renderer_model_man::sync( dpthread_lock *thd, renderer_model_man_ref *g )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_writelock *pwl;
        renderer_model_readlock *pl;
        shared_obj_guard o, o1, o2;
        renderer_model_man_readlock *grl;
        renderer_model_man_writelock *gwl;
        uint64_t t;
        model_man_readlock *mrl;
        std::list<model *> lg, ng;
        std::list<model *>::iterator ig;
        model *pg;
        dpid_btree pt;
        model_readlock *gl;
        model_writelock *gw;
        
        grl = (renderer_model_man_readlock *)o1.tryReadLock( g, 100, "renderer_model_man::sync" );
        if( !grl )
            return;
        t = thd->getTicks();
        if( t - grl->t->t_last_model_synced < 100 )
            return;
        grl->t->t_last_model_synced = t;
        
        //build list of models
        l = &grl->t->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::sync" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), p );
        }
        o.unlock();
        
        mrl = (model_man_readlock *)o2.tryReadLock( grl->t->g_models, 100, "renderer_model_man::sync" );
        if( !mrl )
            return;
        
        //find models, sync them, and remove from list
        mrl->getModels( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            gl = (model_readlock *)o.tryReadLock( pg, 1000, "renderer_model_man::sync" );
            if( !gl )
                continue;
            p = (renderer_model *)pt.findLeaf( gl->getId() );
            if( !p )
                ng.push_back( pg );
            else
                pt.removeLeaf( p );
        }
        o.unlock();
        
        //build list of models to delete
        l = &grl->t->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::sync" );
            if( !pl )
                continue;
            if( pt.findLeaf( pl->getId() ) )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() && ng.empty() )
            return;
        
        gwl = (renderer_model_man_writelock *)o1.tryWriteLock( g, 100, "renderer_model_man::sync" );
        if( !gwl )
            return;
        
        //kill unmatched models
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pwl = (renderer_model_writelock *)o.tryWriteLock( p, 100, "renderer_model_man::sync" );
            if( !pwl )
                continue;
            pwl->kill();
        }
        o.unlock();
        
        //create new models
        for( ig = ng.begin(); ig != ng.end(); ++ig )
        {
            pg = *ig;
            gw = (model_writelock *)o.tryWriteLock( pg, 1000, "renderer_model_man::sync" );
            if( !gw )
                continue;
            p = gwl->t->genModel( gw );
            gw->setRenderer( p );
            gwl->t->models.push_back( p );
        }
        o.unlock();
        
    }
    
    //run from manager thread
    void renderer_model_man::runFromTask( dpthread_lock *thd, renderer_model_man_ref *g )
    {
        std::list<renderer_model *> *l;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_readlock *pl;
        shared_obj_guard o, o1;
        renderer_model_man_readlock *grl;
        
        grl = (renderer_model_man_readlock *)o1.tryReadLock( g, 100, "renderer_model_man::runFromTask" );
        if( !grl )
            return;
        
        l = &grl->t->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::runFromTask" );
            if( !pl )
                continue;
            pl->runFromTask( thd );
        }
        o.unlock();
        
        renderer_model_man::sync( thd, g );
    }
    
    //run from renderer thread
    void renderer_model_man::runFromRenderer( dpthread_lock *thd, renderer_model_man *g )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_readlock *pl;
        shared_obj_guard o, o1;
        renderer_model_man_readlock *grl;
        renderer_model_man_writelock *gwl;
        
        grl = (renderer_model_man_readlock *)o1.tryReadLock( g, 100, "renderer_model_man::runFromTask" );
        if( !grl )
            return;
        
        l = &grl->t->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::runFromRenderer" );
            if( !pl )
                continue;
            pl->runFromRenderer( thd );
            if( !pl->isAlive() )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        gwl = (renderer_model_man_writelock *)o1.tryReadLock( g, 100, "renderer_model_man::runFromTask" );
        if( !gwl )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            gwl->t->models.remove( p );
            delete p;
        }
    }
    
    //delete models
    void renderer_model_man::deleteModels( void )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }
    
    //render models
    void renderer_model_man::renderModels( dpthread_lock *thd, renderer_writelock *rl, renderer_model_man_readlock *ml, dpmatrix *m_world )
    {
        std::list<renderer_model *> *l;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_readlock *pl;
        shared_obj_guard o;
        
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::rendermodels" );
            if( !pl )
               continue;
            pl->render( thd, rl, 0, m_world );
        }
    }
    
    //generate renderer model
    renderer_model *renderer_model_man::genModel( model_writelock *ml )
    {
        return 0;
    }
    
};
