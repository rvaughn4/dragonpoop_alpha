
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
#include "../renderer/renderers.h"
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
#include "dpland/dpland_man.h"
#include "dpactor/dpactor_man.h"
#include "dpactor/dpactor_man_readlock.h"
#include "dpactor/dpactor_man_writelock.h"
#include "model/model_man.h"
#include "model/model_man_readlock.h"
#include "model/model_man_writelock.h"

namespace dragonpoop
{

    //ctor
    gfx::gfx( core *c, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        gfx_writelock *l;
        shared_obj_guard o;
        
        this->root_g = 0;
        this->root_factory = 0;
        this->gui_cnt = 0;
        this->model_cnt = 0;

        this->c = c;
        this->r = 0;
        this->_startTasks( c, tp );
        this->tpr = (dptaskpool_ref *)tp->getRef();

        this->model_mgr = new model_man( c, this, tp );
        this->land_mgr = new dpland_man( c, this, tp );
        this->actor_mgr = new dpactor_man( c, this, tp );
        
        l = (gfx_writelock *)o.writeLock( this, "gfx::gfx" );
        this->r = new openglx_1o5_renderer( c, l, tp );
    }

    //dtor
    gfx::~gfx( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "gfx::~gfx" );
        o.unlock();
        this->unlink();
        
        this->kill();
        this->_deleteTasks();
        
        o.tryWriteLock( this, 5000, "gfx::~gfx" );
        
        delete this->actor_mgr;
        delete this->land_mgr;
        delete this->model_mgr;
        
        this->deleteGuis();

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

    //start all tasks
    void gfx::_startTasks( core *c, dptaskpool_writelock *tp )
    {
        this->_startTask( c, tp, &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk, 1, 0, 0, 0, 100 );
        this->_startTask( c, tp, &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk, 0, 0, 1, 0, 50 );
        this->bIsRun = 1;
    }
    
    //start task
    void gfx::_startTask( core *c, dptaskpool_writelock *tp, gfx_task **pgtsk, dptask **ptsk, bool bRunGfx, bool bRunModels, bool bRunGuis, bool bRunActors, unsigned int ms_delay )
    {
        *pgtsk = new gfx_task( this, bRunGfx, bRunModels, bRunGuis, bRunActors );
        *ptsk = new dptask( c->getMutexMaster(), *pgtsk, ms_delay, 0, "gfx" );
        tp->addTask( *ptsk );
    }
    
    //kill all tasks
    void gfx::_killTasks( void )
    {
        this->_killTask( &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk );
        this->_killTask( &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk );
        this->bIsRun = 0;
    }
    
    //kill task
    void gfx::_killTask( gfx_task **pgtsk, dptask **ptsk )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !*ptsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( *ptsk, "gfx::genRef" );
        tl->kill();
        o.unlock();
    }
    
    //delete all tasks
    void gfx::_deleteTasks( void )
    {
        this->_deleteTask( &this->tasks.tgfx.gtsk, &this->tasks.tgfx.tsk );
        this->_deleteTask( &this->tasks.tguis.gtsk, &this->tasks.tguis.tsk );
    }
    
    //delete task
    void gfx::_deleteTask( gfx_task **pgtsk, dptask **ptsk )
    {
        if( *ptsk )
            delete *ptsk;
        *ptsk = 0;
        if( *pgtsk )
            delete *pgtsk;
        *pgtsk = 0;
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

    //stop gfx task
    void gfx::kill( void )
    {
        this->_killTasks();
    }

    //run gfx from task
    void gfx::run( dpthread_lock *thd, gfx_writelock *g )
    {
        uint64_t t;
        shared_obj_guard o;
        renderer_readlock *l;
        gui_factory_writelock *fl;
        
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
        
        if( !this->root_g && this->root_factory )
        {
            fl = (gui_factory_writelock *)o.tryWriteLock( this->root_factory, 10, "gfx::run" );
            if( fl )
            {
                this->root_g = fl->makeGui( thd, 0 );
                if( this->root_g )
                    this->addGui( this->root_g );
            }
            o.unlock();
        }

    }


    //delete all guis
    void gfx::deleteGuis( void )
    {
        std::list<gui_ref *> *l, d;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        
        l = &this->guis;
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
    
    //run all guis
    void gfx::runGuis( dpthread_lock *thd, gfx_ref *g )
    {
        std::list<gui_ref *> *l, ll, d;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        shared_obj_guard o, og;
        gui_writelock *pl;
        gfx_readlock *grl;
        gfx_writelock *gwl;
        
        grl = (gfx_readlock *)og.tryReadLock( g, 100, "gfx::runGuis" );
        if( !grl )
            return;
        
        l = &grl->t->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
            else
            {
                pl = (gui_writelock *)o.tryWriteLock( p, 100, "gfx::runGuis" );
                if( pl )
                {
                    p = (gui_ref *)pl->getRef();
                    if( p )
                    ll.push_back( p );
                }
                o.unlock();
            }
        }
        og.unlock();
        
        l = &ll;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (gui_writelock *)o.tryWriteLock( p, 100, "gfx::runGuis" );
            if( pl )
                pl->run( thd );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        gwl = (gfx_writelock *)og.tryReadLock( g, 100, "gfx::runGuis" );
        if( !gwl )
            return;

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            gwl->t->guis.remove( p );
            delete p;
            gwl->t->gui_cnt--;
        }
        og.unlock();
    }
    
    //add gui
    void gfx::addGui( gui *g )
    {
        shared_obj_guard o;
        gui_writelock *l;
        gui_ref *r;
        
        l = (gui_writelock *)o.writeLock( g, "gfx::addGui" );
        if( !l )
            return;
        
        r = (gui_ref *)l->getRef();
        if( r )
        {
            this->guis.push_back( r );
            this->gui_cnt++;
        }
    }
    
    //add gui
    void gfx::addGui( gui_ref *g )
    {
        shared_obj_guard o;
        gui_writelock *l;
        gui_ref *r;
        
        l = (gui_writelock *)o.writeLock( g, "gfx::addGui" );
        if( !l )
            return;
        
        r = (gui_ref *)l->getRef();
        if( r )
        {
            this->guis.push_back( r );
            this->gui_cnt++;
        }
    }
    
    //get guis
    void gfx::getGuis( std::list<gui_ref *> *ll )
    {
        std::list<gui_ref *> *l;
        std::list<gui_ref *>::iterator i;
        gui_ref *p;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            ll->push_back( p );
        }
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
    
    //set root gui factory
    void gfx::setRootGui( gui_factory *g )
    {
        shared_obj_guard o;
        gui_factory_writelock *gl;
        
        if( this->root_g )
            delete this->root_g;
        this->root_g = 0;
        if( this->root_factory )
            delete this->root_factory;
        this->root_factory = 0;
        
        gl = (gui_factory_writelock *)o.tryWriteLock( g, 2000, "gfx::setRootGui" );
        if( !gl )
            return;
        this->root_factory = (gui_factory_ref *)gl->getRef();
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
    
    //return model count
    unsigned int gfx::getModelCount( void )
    {
        return this->model_cnt;
    }
    
    //return gui count
    unsigned int gfx::getGuiCount( void )
    {
        return this->gui_cnt;
    }
    
    //get camera position
    void gfx::getCameraPosition( dpposition *p )
    {
        p->copy( &this->cam_pos );
    }
    
    //set camera position
    void gfx::setCameraPosition( dpposition *p )
    {
        shared_obj_guard o;
        renderer_writelock *l;
        
        this->cam_pos.copy( p );

        if( !this->r )
            return;
        l = (renderer_writelock *)o.tryWriteLock( this->r, 1000, "gfx::setCameraPosition" );
        if( !l )
            return;
        l->syncCamera();
    }
    
    //add actor
    void gfx::addActor( dpactor *a )
    {
        shared_obj_guard o;
        dpactor_man_writelock *l;
        
        l = (dpactor_man_writelock *)o.tryWriteLock( this->actor_mgr, 500, "gfx::addActor" );
        if( !l )
            return;
        
        return l->addActor( a );
    }
    
    //add actor
    void gfx::addActor( dpactor_ref *a )
    {
        shared_obj_guard o;
        dpactor_man_writelock *l;
        
        l = (dpactor_man_writelock *)o.tryWriteLock( this->actor_mgr, 500, "gfx::addActor" );
        if( !l )
            return;
        
        return l->addActor( a );
    }
    
    //return actor count
    unsigned int gfx::getActorCount( void )
    {
        shared_obj_guard o;
        dpactor_man_readlock *l;
        
        l = (dpactor_man_readlock *)o.tryReadLock( this->actor_mgr, 500, "gfx::getActorCount" );
        if( !l )
            return 0;
        
        return l->getActorCount();
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
    
};
