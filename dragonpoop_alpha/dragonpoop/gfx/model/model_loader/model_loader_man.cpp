
#include "model_loader_man.h"
#include "model_loader_man_ref.h"
#include "model_loader_man_readlock.h"
#include "model_loader_man_writelock.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/dptask/dptask_writelock.h"
#include "../../../core/dptask/dptask.h"
#include "../../../core/dptaskpool/dptaskpool_writelock.h"
#include "model_loader_man_task.h"
#include "../../gfx_ref.h"
#include "../../gfx.h"
#include "../../gfx_writelock.h"
#include "model_loader.h"
#include "model_loader_ref.h"
#include "model_loader_readlock.h"
#include "model_loader_writelock.h"
#include "model_saver.h"
#include "model_saver_ref.h"
#include "model_saver_readlock.h"
#include "model_saver_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_man::model_loader_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        
        this->c = c;
        this->loader_cnt = 0;
        this->saver_cnt = 0;
        
        gl = (gfx_writelock *)o.writeLock( g, "model_loader_man::model_loader_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();
        
        this->_startTask( tp, 200 );
    }
    
    //dtor
    model_loader_man::~model_loader_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "model_loader_man::~model_loader_man" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "model_loader_man::~model_loader_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "model_loader_man::~model_loader_man" );
        this->_deleteTask();
        this->deleteLoaders();
        this->deleteSavers();
        delete this->g;
        o.unlock();
    }
    
    //return core
    core *model_loader_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *model_loader_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_loader_man_readlock( (model_loader_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *model_loader_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_loader_man_writelock( (model_loader_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *model_loader_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_loader_man_ref( (model_loader_man *)p, k );
    }
    
    //start task
    void model_loader_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new model_loader_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "model_loader_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void model_loader_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "model_loader_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void model_loader_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //run
    void model_loader_man::run( dpthread_lock *thd, model_loader_man_writelock *g )
    {
        this->runLoaders( thd );
        this->runSavers( thd );
    }
    
    //delete all loaders
    void model_loader_man::deleteLoaders( void )
    {
        std::list<model_loader *> *l, d;
        std::list<model_loader *>::iterator i;
        model_loader *p;
        
        l = &this->loaders;
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
    
    //delete all savers
    void model_loader_man::deleteSavers( void )
    {
        std::list<model_saver *> *l, d;
        std::list<model_saver *>::iterator i;
        model_saver *p;
        
        l = &this->savers;
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
    
    //run all loaders
    void model_loader_man::runLoaders( dpthread_lock *thd )
    {
        std::list<model_loader *> *l, ll, d;
        std::list<model_loader *>::iterator i;
        model_loader *p;
        shared_obj_guard o;
        model_loader_writelock *pl;
        
        l = &this->loaders;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_loader_writelock *)o.tryWriteLock( p, 100, "model_loader_man::runLoaders" );
            if( !pl )
                continue;
            pl->run( thd );
            if( !pl->isRunning() )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->loaders.remove( p );
            this->loader_cnt--;
            delete p;
        }
    }
    
    //run all savers
    void model_loader_man::runSavers( dpthread_lock *thd )
    {
        std::list<model_saver *> *l, ll, d;
        std::list<model_saver *>::iterator i;
        model_saver *p;
        shared_obj_guard o;
        model_saver_writelock *pl;
        
        l = &this->savers;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_saver_writelock *)o.tryWriteLock( p, 100, "model_loader_man::runSavers" );
            if( !pl )
                continue;
            pl->run( thd );
            if( !pl->isRunning() )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->savers.remove( p );
            this->saver_cnt--;
            delete p;
        }
    }
    
    //load file into model
    bool model_loader_man::load( model_ref *m, const char *path_name, const char *file_name, model_loader_ref **mldr )
    {
        model_loader *l;
        model_loader_writelock *lw;
        shared_obj_guard o;
        
        l = model_loader::loadFile( this->c, m, path_name, file_name );
        if( !l )
            return 0;
        
        if( mldr )
        {
            lw = (model_loader_writelock *)o.tryWriteLock( l, 1000, "model_loader_man::load" );
            if( lw )
                *mldr = (model_loader_ref *)lw->getRef();
            else
                *mldr = 0;
            o.unlock();
        }
        
        this->loaders.push_back( l );
        return 1;
    }
    
    //save model into file
    bool model_loader_man::save( model_ref *m, const char *path_name, const char *file_name, model_saver_ref **mldr )
    {
        model_saver *l;
        model_saver_writelock *lw;
        shared_obj_guard o;
        
        l = model_saver::saveFile( this->c, m, path_name, file_name );
        if( !l )
            return 0;
        
        if( mldr )
        {
            lw = (model_saver_writelock *)o.tryWriteLock( l, 1000, "model_loader_man::save" );
            if( lw )
                *mldr = (model_saver_ref *)lw->getRef();
            else
                *mldr = 0;
            o.unlock();
        }
        
        this->savers.push_back( l );
        return 1;
    }
    
};
