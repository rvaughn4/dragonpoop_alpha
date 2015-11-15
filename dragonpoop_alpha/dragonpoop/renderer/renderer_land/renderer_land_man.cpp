
#include "renderer_land_man.h"
#include "renderer_land_man_ref.h"
#include "renderer_land_man_readlock.h"
#include "renderer_land_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "renderer_land_man_task.h"
#include "../../core/dptaskpool/dptaskpool_ref.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "../../core/dptaskpool/dptaskpool_readlock.h"
#include "../../core/dpthread/dpthread_singletask.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "../renderer.h"
#include "../renderer_ref.h"
#include "../renderer_readlock.h"
#include "../renderer_writelock.h"
#include "../../gfx/gfx_writelock.h"
#include "../../gfx/gfx_ref.h"
#include "../../gfx/dpland/dpland_man.h"
#include "../../gfx/dpland/dpland_man_ref.h"
#include "../../gfx/dpland/dpland_man_readlock.h"
#include "renderer_land.h"
#include "../../gfx/dpland/dpland.h"
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

#include <math.h>
#include <thread>

namespace dragonpoop
{

    //ctor
    renderer_land_man::renderer_land_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o, o1;
        gfx_writelock *gl;
        renderer_writelock *rl;
        renderer_commandlist_passer_writelock *cpl;

        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( clpasser, 5000, "renderer_land_man::renderer_land_man" );
        if( cpl )
            this->clpasser = (renderer_commandlist_passer_ref *)cpl->getRef();
        else
            this->clpasser = 0;
        o.unlock();

        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_land_man::renderer_land_man" );
        if( gl )
            gl->getLand( &this->g_lands );
        o.unlock();

        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        this->grass = 0;

        rl = (renderer_writelock *)o.writeLock( r, "renderer_land_man::renderer_land_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();

        this->thd = new dpthread_singletask( c->getMutexMaster(), 302 );
        this->_startTask( tp, 30 );
    }

    //dtor
    renderer_land_man::~renderer_land_man( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->deleteLands();
        o.unlock();
        this->unlink();
        delete this->thd;

        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_lands;
        delete this->tpr;
        delete this->clpasser;
        o.unlock();
    }

    //compute matrix
    void renderer_land_man::computeMatrix( void )
    {
        this->m.setIdentity();
    }

    //return core
    core *renderer_land_man::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *renderer_land_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_land_man_readlock( (renderer_land_man *)p, l );
    }

    //generate write lock
    shared_obj_writelock *renderer_land_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_land_man_writelock( (renderer_land_man *)p, l );
    }

    //generate ref
    shared_obj_ref *renderer_land_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_land_man_ref( (renderer_land_man *)p, k );
    }

    //start task
    void renderer_land_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        dpthread_lock *thdl;

        this->gtsk = new renderer_land_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, "renderer_land_man" );

        thdl = this->thd->lock();
        if( thdl )
        {
            thdl->addTask( this->tsk );
            delete thdl;
        }
    }

    //kill task
    void renderer_land_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk, "renderer_land_man::_killTask" );
        tl->kill();
        o.unlock();
    }

    //delete task
    void renderer_land_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }

    //sync lands
    void renderer_land_man::sync( dpthread_lock *thd )
    {
        std::list<renderer_land *> *l, d;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;
        shared_obj_guard o2;
        uint64_t t;
        dpland_man_readlock *mrl;
        std::list<dpland *> lg, ng;
        std::list<dpland *>::iterator ig;
        dpland *pg;
        dpid_btree pt;
        unsigned int new_ctr;

        t = thd->getTicks();
        if( t - this->t_last_land_synced < 30 )
            return;
        this->t_last_land_synced = t;

        //build list of lands
        l = &this->lands;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pt.addLeaf( p->getId(), p );
        }

        mrl = (dpland_man_readlock *)o2.tryReadLock( this->g_lands, 10, "renderer_land_man::sync" );
        if( !mrl )
            return;

        //find lands, sync them, and remove from list
        mrl->getTiles( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            p = (renderer_land *)pt.findLeaf( pg->getId() );
            if( !p )
                ng.push_back( pg );
            else
                pt.removeLeaf( p );
        }

        //build list of lands to delete
        l = &this->lands;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( pt.findLeaf( p->getId() ) )
                d.push_back( p );
        }

        if( d.empty() && ng.empty() )
            return;

        //kill unmatched lands
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->lands.remove( p );
            delete p;
        }

        //create new lands
        new_ctr = 0;
        for( ig = ng.begin(); ig != ng.end(); ++ig )
        {
            pg = *ig;
            if( new_ctr > 1 )
                continue;
            new_ctr++;
            p = this->genLand( pg, thd );
            if( p )
                this->lands.push_back( p );
        }
    }

    //run lands
    void renderer_land_man::runLands( dpthread_lock *thd )
    {
        this->computeMatrix();
    }

    //run from manager thread
    void renderer_land_man::run( dpthread_lock *thd, renderer_land_man_writelock *l )
    {
        this->sync( thd );
        this->runLands( thd );
        this->render( thd );
    }

    //delete lands
    void renderer_land_man::deleteLands( void )
    {
        std::list<renderer_land *> *l, d;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;

        l = &this->lands;
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

    //render into command list
    void renderer_land_man::render( dpthread_lock *thd )
    {
        shared_obj_guard o, octxt, ocl;
        render_api_context_writelock *ctxl;
        render_api_commandlist_writelock *cll;
        renderer_commandlist_passer_writelock *cpl;
        shared_obj_guard ocpl;
        render_api_shader_ref *sdr;

        if( this->clpasser->t->land_ready )
            return;

        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( this->ctx, 100, "renderer_land_man::render" );
        if( !ctxl )
            return;

        ctxl->makeActive( thd );
        if( this->clist )
            delete this->clist;
        this->clist = ctxl->makeCmdList();
        if( !this->clist )
            return;

        cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->clist, 100, "renderer_land_man::render" );
        if( !cll )
            return;

        sdr = ctxl->makeShader( "land" );
        if( !sdr )
            return;

        cll->setShader( sdr );
        this->renderLands( thd, &this->campos, &this->m, ctxl, cll );

        cll->setPosition( &this->campos );
        if( cll->compile( ctxl ) )
        {
            cpl = (renderer_commandlist_passer_writelock *)ocpl.tryWriteLock( this->clpasser, 100, "renderer_land_man::render" );
            if( cpl )
            {
                cpl->setLand( this->clist );
                this->campos.copy( cpl->getPosition() );
                this->clpasser->t->land_ready = 1;
                ocpl.unlock();
            }
        }

        delete sdr;
    }

    //render lands
    void renderer_land_man::renderLands( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl )
    {
        std::list<renderer_land *> *l;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;

        if( !this->grass )
        {
            dpbitmap bm;
            bm.loadFile( "grass.bmp" );
            this->grass = ctx->makeTexture( &bm );
        }

        cl->setTexture( this->grass, 0 );

        l = &this->lands;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->render( thd, campos, m_world, ctx, cl );
        }
    }

    //generate renderer land
    renderer_land *renderer_land_man::genLand( dpland *ml, dpthread_lock *thd )
    {
        render_api_context_writelock *ctx;
        shared_obj_guard o;

        ctx = (render_api_context_writelock *)o.tryWriteLock( this->ctx, 10, "renderer_land_man::genLand" );
        if( !ctx )
            return 0;

        ctx->makeActive( thd );
        return new renderer_land( ml, ctx );
    }

};
