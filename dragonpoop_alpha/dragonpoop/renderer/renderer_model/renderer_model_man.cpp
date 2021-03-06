
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
#include "../../core/dpthread/dpthread_singletask.h"
#include "../renderer.h"
#include "../renderer_ref.h"
#include "../renderer_readlock.h"
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
#include "../api_stuff/render_api/render_api_shader_ref.h"
#include "../api_stuff/render_api/render_api_context_ref.h"
#include "../api_stuff/render_api/render_api_context_writelock.h"
#include "../api_stuff/render_api/render_api_context_readlock.h"
#include "../api_stuff/render_api/render_api_commandlist_ref.h"
#include "../api_stuff/render_api/render_api_commandlist_writelock.h"
#include "../renderer_commandlist_passer.h"
#include "../renderer_commandlist_passer_ref.h"
#include "../renderer_commandlist_passer_writelock.h"
#include "../../gfx/dpheight_cache/dpheight_cache.h"
#include "../../gfx/dpheight_cache/dpheight_cache_ref.h"
#include "../../gfx/dpheight_cache/dpheight_cache_writelock.h"
#include "../../gfx/dpheight_cache/dpheight_cache_readlock.h"

#include <thread>

namespace dragonpoop
{

    //ctor
    renderer_model_man::renderer_model_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        renderer_writelock *rl;
        renderer_commandlist_passer_writelock *cpl;

        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( clpasser, 5000, "renderer_model_man::renderer_model_man" );
        if( cpl )
            this->clpasser = (renderer_commandlist_passer_ref *)cpl->getRef();
        else
            this->clpasser = 0;

        this->listReady = 1;
        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();

        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_model_man::renderer_model_man" );
        if( gl )
        {
            gl->getModels( &this->g_models );
            this->heights = gl->getHeights();
        }
        o.unlock();
        this->local_heights = new dpheight_cache( c->getMutexMaster() );

        rl = (renderer_writelock *)o.writeLock( r, "renderer_model_man::renderer_model_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();

        this->_startTask( tp, 5 );
    }

    //dtor
    renderer_model_man::~renderer_model_man( void )
    {
        shared_obj_guard o;
        renderer_commandlist_passer_ref *r;

        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->deleteModels();
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "renderer_model_man::~renderer_model_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_models;
        delete this->tpr;
        delete this->heights;
        delete this->local_heights;
        r = this->clpasser;
        delete r;
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
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, 1, "renderer_model_man" );
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
    void renderer_model_man::sync( dpthread_lock *thd, renderer_model_man_writelock *g )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_writelock *pwl;
        renderer_model_readlock *pl;
        shared_obj_guard o, o2;
        uint64_t t;
        model_man_readlock *mrl;
        std::list<model *> lg, ng;
        std::list<model *>::iterator ig;
        model *pg;
        dpid_btree pt;
        model_readlock *gl;
        model_writelock *gw;

        t = thd->getTicks();
        if( t - this->t_last_model_synced < 100 )
            return;
        this->t_last_model_synced = t;

        //build list of models
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer_model_man::sync" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), p );
        }
        o.unlock();

        mrl = (model_man_readlock *)o2.tryReadLock( this->g_models, 100, "renderer_model_man::sync" );
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
        l = &this->models;
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
            p = this->genModel( gw );
            gw->setRenderer( p );
            this->models.push_back( p );
        }
        o.unlock();

    }

    //delete old models
    void renderer_model_man::deleteOldModels( dpthread_lock *thd, renderer_model_man_writelock *g )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_writelock *pl;
        shared_obj_guard o;

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_writelock *)o.tryWriteLock( p, 100, "renderer_model_man::deleteOldModels" );
            if( !pl )
                continue;
            if( !pl->isAlive() )
                d.push_back( p );
        }
        o.unlock();

        if( d.empty() )
            return;

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->models.remove( p );
            delete p;
        }
    }

    //run guis
    void renderer_model_man::runModels( dpthread_lock *thd, renderer_model_man_writelock *g )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_writelock *pl;
        shared_obj_guard o, oc;
        render_api_context_writelock *ctxl;

        ctxl = (render_api_context_writelock *)oc.tryWriteLock( this->ctx, 100, "renderer_model_man::runModels" );
        if( !ctxl )
            return;

        ctxl->makeActive( thd );
        this->computeMatrix();

        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_writelock *)o.tryWriteLock( p, 100, "renderer_model_man::runModels" );
            if( !pl )
                continue;
            pl->run( thd, ctxl );
        }
        o.unlock();
        oc.unlock();
    }

    //run from manager thread
    void renderer_model_man::run( dpthread_lock *thd, renderer_model_man_writelock *g )
    {
        dpheight_cache_writelock *l;
        dpheight_cache_readlock *lr;
        shared_obj_guard o;

        l = (dpheight_cache_writelock *)o.tryWriteLock( this->local_heights, 100, "renderer_model_man::run" );
        if( l )
        {
            l->sync( this->heights );
            o.unlock();
        }

        lr = (dpheight_cache_readlock *)o.tryReadLock( this->local_heights, 100, "renderer_model_man::run" );
        if( lr )
        {
            this->render( thd, g, lr );
            o.unlock();
        }

        this->sync( thd, g );
        this->runModels( thd, g );
        this->deleteOldModels( thd, g );
    }

    //render guis into commandlist
    void renderer_model_man::render( dpthread_lock *thd, renderer_model_man_writelock *g, dpheight_cache_readlock *heights )
    {
        shared_obj_guard o, octxt, ocl;
        render_api_context_writelock *ctxl;
        render_api_commandlist_writelock *cll;
        renderer_commandlist_passer_writelock *cpl;
        shared_obj_guard ocpl;
        render_api_shader_ref *sdr;

        if( renderer_commandlist_passer::waitForFlag( &this->clpasser->t->model_ready, 0, 5 ) )
            return;

        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( this->ctx, 100, "renderer_model_man::render" );
        if( !ctxl )
            return;

        ctxl->makeActive( thd );
        ctxl->run();
        if( this->clist )
            delete this->clist;
        this->clist = ctxl->makeCmdList();
        if( !this->clist )
            return;

        cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->clist, 100, "renderer_model_man::render" );
        if( !cll )
            return;

        sdr = ctxl->makeShader( "model" );
        if( !sdr )
            return;

        cll->setShader( sdr );
        this->renderModels( thd, &this->campos, heights, &this->m, ctxl, cll );

        cll->setPosition( &this->campos );
        if( cll->compile( ctxl ) )
        {
            cpl = (renderer_commandlist_passer_writelock *)ocpl.tryWriteLock( this->clpasser, 100, "renderer_model_man::swapList" );
            if( cpl )
            {
                cpl->setModel( this->clist );
                this->campos.copy( cpl->getPosition() );
                this->clpasser->t->model_ready = 1;
                ocpl.unlock();
            }
        }

        delete sdr;
    }

    //compute matrix
    void renderer_model_man::computeMatrix( void )
    {
        this->m.setIdentity();
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
    void renderer_model_man::renderModels( dpthread_lock *thd, dpposition *campos, dpheight_cache_readlock *heights, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
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
            pl->render( thd, campos, heights, m_world, ctx, clist );
        }
    }

    //generate renderer model
    renderer_model *renderer_model_man::genModel( model_writelock *ml )
    {
        return new renderer_model( ml );
    }

};
