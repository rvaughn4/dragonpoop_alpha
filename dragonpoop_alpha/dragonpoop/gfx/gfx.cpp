
#include "gfx.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "gfx_task.h"
#include "gfx_readlock.h"
#include "gfx_writelock.h"
#include "gfx_ref.h"
#include "../core/dpthread/dpthread_lock.h"
#include "../core/dptaskpool/dptaskpool_writelock.h"
#include "../core/dptaskpool/dptaskpool_readlock.h"
#include "../core/dptaskpool/dptaskpool_ref.h"
#include "../renderer/renderer.h"
#include "../renderer/renderer_ref.h"
#include "../renderer/renderer_writelock.h"
#include "../renderer/renderer_readlock.h"
#include "../renderer/renderer_factory.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "model/model.h"
#include "model/model_ref.h"
#include "model/model_writelock.h"
#include "model/model_readlock.h"
#include "model/model_instance/model_instance_ref.h"
#include "model/model_instance/model_instance_writelock.h"
#include "gui/gui_ref.h"
#include "gui/gui.h"
#include "gui/gui_writelock.h"
#include "gui/gui_factory.h"
#include "gui/gui_factory_ref.h"
#include "gui/gui_factory_writelock.h"
#include "gui/root_gui/root_gui_factory.h"
#include "dpactor/dpactor.h"
#include "dpactor/dpactor_ref.h"
#include "dpactor/dpactor_writelock.h"
#include "dpactor/dpactor_man.h"
#include "dpactor/dpactor_man_readlock.h"
#include "dpactor/dpactor_man_writelock.h"
#include "model/model_man.h"
#include "model/model_man_readlock.h"
#include "model/model_man_writelock.h"
#include "gui/gui_man.h"
#include "gui/gui_man_readlock.h"
#include "gui/gui_man_writelock.h"
#include "dpland/dpland_man.h"
#include "dpland/dpland_man_ref.h"
#include "dpland/dpland_man_readlock.h"
#include "dpland/dpland_man_writelock.h"
#include "dpsky/dpsky_man.h"
#include "dpsky/dpsky_man_ref.h"
#include "dpsky/dpsky_man_readlock.h"
#include "dpsky/dpsky_man_writelock.h"
#include "dpposition/dpposition_share.h"
#include "dpposition/dpposition_share_ref.h"
#include "dpposition/dpposition_share_writelock.h"
#include "dpheight_cache/dpheight_cache.h"
#include "dpheight_cache/dpheight_cache_writelock.h"
#include "dpheight_cache/dpheight_cache_ref.h"

namespace dragonpoop
{

    //ctor
    gfx::gfx( core *c, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        this->root_g = 0;
        this->root_factory = 0;
        this->gui_cnt = 0;
        this->model_cnt = 0;

        this->cam_pos = new dpposition_share( c->getMutexMaster() );
        this->heights = new dpheight_cache( c->getMutexMaster() );
        this->c = c;
        this->r = 0;
        this->_startTask( c, tp );
        this->tpr = (dptaskpool_ref *)tp->getRef();

        this->model_mgr = new model_man( c, this, tp );
        this->land_mgr = new dpland_man( c, this, tp );
        this->actor_mgr = new dpactor_man( c, this, tp );
        this->gui_mgr = new gui_man( c, this, tp );
        this->sky_man = new dpsky_man( c, this, tp );
        this->r = 0;

        renderer::addRenderers( this );
    }

    //dtor
    gfx::~gfx( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "gfx::~gfx" );
        o.unlock();
        this->unlink();

        this->_killTask();
        this->_deleteTask();

        o.tryWriteLock( this, 5000, "gfx::~gfx" );

        delete this->gui_mgr;
        delete this->actor_mgr;
        delete this->land_mgr;
        delete this->model_mgr;
        delete this->sky_man;
        delete this->heights;
        delete this->cam_pos;

        if( this->root_g )
            delete this->root_g;
        this->root_g = 0;
        if( this->root_factory )
            delete this->root_factory;
        this->root_factory = 0;

        delete this->r;
        this->r = 0;
        o.unlock();
    }

    //start task
    void gfx::_startTask( core *c, dptaskpool_writelock *tp )
    {
        this->gtsk = new gfx_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 100, 0, "gfx" );
        tp->addTask( this->tsk );
    }

    //stop gfx task
    void gfx::kill( void )
    {
        this->_killTask();
    }

    //kill task
    void gfx::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk, "_gfx::killTask" );
        tl->kill();
        o.unlock();
    }

    //delete task
    void gfx::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }

    //returns true if running
    bool gfx::isRunning( void )
    {
        return this->bIsRun;
    }

    //return core
    core *gfx::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *gfx::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gfx_readlock( (gfx *)p, l );
    }

    //generate write lock
    shared_obj_writelock *gfx::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gfx_writelock( (gfx *)p, l );
    }

    //generate ref
    shared_obj_ref *gfx::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gfx_ref( (gfx *)p, k );
    }

    //run gfx from task
    void gfx::run( dpthread_lock *thd, gfx_writelock *g )
    {
        uint64_t t;
        shared_obj_guard o;
        renderer_readlock *l;

        t = thd->getTicks();
        if( this->r && t - this->last_r_poll > 4000 )
        {
            l = (renderer_readlock *)o.tryReadLock( this->r, 3, "gfx::run" );
            if( l )
            {
                this->ms_each_frame = l->getMsPerFrame();
                this->fps = l->getFps();
                this->last_r_poll = t;
            }
            o.unlock();
        }

        if( !this->r )
            this->changeRenderer( 0 );

    }

    //return fps
    float gfx::getFps( void )
    {
        return this->fps;
    }

    //return ms each frame
    unsigned int gfx::getMsEachFrame( void )
    {
        return this->ms_each_frame;
    }

    //return renderer
    renderer_ref *gfx::getRenderer( void )
    {
        shared_obj_guard o;
        renderer_writelock *l;

        if( !this->r )
            return 0;
        l = (renderer_writelock *)o.tryWriteLock( this->r, 1000, "gfx::getRenderer" );
        if( !l )
            return 0;

        return (renderer_ref *)l->getRef();
    }

    //get camera position
    dpposition_share_ref *gfx::getCameraPosition( void )
    {
        shared_obj_guard o;
        dpposition_share_writelock *l;

        l = (dpposition_share_writelock *)o.tryWriteLock( this->cam_pos, 1000, "gfx::getCameraPosition" );
        if( !l )
            return 0;
        return (dpposition_share_ref *)l->getRef();
    }

    //get height cache
    dpheight_cache_ref *gfx::getHeights( void )
    {
        shared_obj_guard o;
        dpheight_cache_writelock *l;

        l = (dpheight_cache_writelock *)o.tryWriteLock( this->heights, 1000, "gfx::getHeights" );
        if( !l )
            return 0;
        return (dpheight_cache_ref *)l->getRef();
    }

    //get models
    bool gfx::getModels( model_man_ref **r )
    {
        shared_obj_guard o;
        model_man_writelock *l;
        if( !this->getModels( &l, &o ) )
            return 0;
        *r = (model_man_ref *)l->getRef();
        return 1;
    }

    //get models
    bool gfx::getModels( model_man_readlock **r, shared_obj_guard *o )
    {
        *r = (model_man_readlock *)o->tryReadLock( this->model_mgr, 1000, "gfx::getModels" );
        return *r != 0;
    }

    //get models
    bool gfx::getModels( model_man_writelock **r, shared_obj_guard *o )
    {
        *r = (model_man_writelock *)o->tryWriteLock( this->model_mgr, 1000, "gfx::getModels" );
        return *r != 0;
    }

    //get guis
    bool gfx::getGuis( gui_man_ref **r )
    {
        shared_obj_guard o;
        gui_man_writelock *l;
        if( !this->getGuis( &l, &o ) )
            return 0;
        *r = (gui_man_ref *)l->getRef();
        return 1;
    }

    //get guis
    bool gfx::getGuis( gui_man_readlock **r, shared_obj_guard *o )
    {
        *r = (gui_man_readlock *)o->tryReadLock( this->gui_mgr, 1000, "gfx::getGuis" );
        return *r != 0;
    }

    //get guis
    bool gfx::getGuis( gui_man_writelock **r, shared_obj_guard *o )
    {
        *r = (gui_man_writelock *)o->tryWriteLock( this->gui_mgr, 1000, "gfx::getGuis" );
        return *r != 0;
    }

    //get actors
    bool gfx::getActors( dpactor_man_ref **r )
    {
        shared_obj_guard o;
        dpactor_man_writelock *l;
        if( !this->getActors( &l, &o ) )
            return 0;
        *r = (dpactor_man_ref *)l->getRef();
        return 1;
    }

    //get actors
    bool gfx::getActors( dpactor_man_readlock **r, shared_obj_guard *o )
    {
        *r = (dpactor_man_readlock *)o->tryReadLock( this->actor_mgr, 1000, "gfx::getActors" );
        return *r != 0;
    }

    //get actors
    bool gfx::getActors( dpactor_man_writelock **r, shared_obj_guard *o )
    {
        *r = (dpactor_man_writelock *)o->tryWriteLock( this->actor_mgr, 1000, "gfx::getActors" );
        return *r != 0;
    }

    //get land
    bool gfx::getLand( dpland_man_ref **r )
    {
        shared_obj_guard o;
        dpland_man_writelock *l;
        if( !this->getLand( &l, &o ) )
            return 0;
        *r = (dpland_man_ref *)l->getRef();
        return 1;
    }

    //get land
    bool gfx::getLand( dpland_man_readlock **r, shared_obj_guard *o )
    {
        *r = (dpland_man_readlock *)o->tryReadLock( this->land_mgr, 1000, "gfx::getLand" );
        return *r != 0;
    }

    //get land
    bool gfx::getLand( dpland_man_writelock **r, shared_obj_guard *o )
    {
        *r = (dpland_man_writelock *)o->tryWriteLock( this->land_mgr, 1000, "gfx::getLand" );
        return *r != 0;
    }

    //get sky
    bool gfx::getSky( dpsky_man_ref **r )
    {
        shared_obj_guard o;
        dpsky_man_writelock *l;
        if( !this->getSky( &l, &o ) )
            return 0;
        *r = (dpsky_man_ref *)l->getRef();
        return 1;
    }

    //get sky
    bool gfx::getSky( dpsky_man_readlock **r, shared_obj_guard *o )
    {
        *r = (dpsky_man_readlock *)o->tryReadLock( this->sky_man, 1000, "gfx::getSky" );
        return *r != 0;
    }

    //get sky
    bool gfx::getSky( dpsky_man_writelock **r, shared_obj_guard *o )
    {
        *r = (dpsky_man_writelock *)o->tryWriteLock( this->sky_man, 1000, "gfx::getSky" );
        return *r != 0;
    }

    //add renderer factory
    void gfx::addRenderer( renderer_factory *f )
    {
        this->renderer_factories.push_back( f );
    }

    //change renderer
    bool gfx::changeRenderer( const char *cname )
    {
        renderer_factory *p, *f;
        int hs, s;
        std::list<renderer_factory *> *l;
        std::list<renderer_factory *>::iterator i;
        std::string sname;
        gfx_writelock *gl;
        dptaskpool_writelock *tp;
        shared_obj_guard og, ot;

        gl = (gfx_writelock *)og.tryWriteLock( this, 5000, "gfx::changeRenderer" );
        if( !gl )
            return 0;
        tp = (dptaskpool_writelock *)ot.tryWriteLock( this->tpr, 5000, "gfx::changeRenderer" );
        if( !tp )
            return 0;

        if( cname )
            sname.assign( cname );
        if( sname.length() < 1 )
            cname = 0;
        f = 0;
        hs = 0;

        l = &this->renderer_factories;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            s = p->getScore();
            if( f && s < hs )
                continue;
            if( cname && p->compareName( &sname ) != 0 )
                continue;
            if( !p->works( this->c, gl, tp ) )
                continue;
            f = p;
            hs = s;
        }

        if( !f )
            return 0;
        if( this->r )
            delete this->r;

        this->r = f->makeRenderer( c, gl, tp );
        return this->r != 0;
    }

    //returns taskpool
    dptaskpool_ref *gfx::getTasks( void )
    {
        shared_obj_guard o;
        dptaskpool_writelock *l;

        l = (dptaskpool_writelock *)o.tryWriteLock( this->tpr, 1000, "gfx::getTasks" );
        if( !l )
            return 0;

        return (dptaskpool_ref *)l->getRef();
    }

};
