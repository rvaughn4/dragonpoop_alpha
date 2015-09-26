
#include "renderer.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "renderer_task.h"
#include "renderer_readlock.h"
#include "renderer_writelock.h"
#include "renderer_ref.h"
#include "../core/dpthread/dpthread_lock.h"
#include "../core/dptaskpool/dptaskpool_writelock.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "../gfx/gfx_writelock.h"
#include "../gfx/gfx_readlock.h"
#include "../gfx/gfx_ref.h"
#include "../gfx/gfx.h"
#include <thread>
#include <random>
#include "renderer_model/renderer_model.h"
#include "renderer_model/renderer_model_writelock.h"
#include "renderer_model/renderer_model_readlock.h"
#include "../core/bytetree/dpid_bytetree.h"
#include "../gfx/model/model_ref.h"
#include "../gfx/model/model.h"
#include "../gfx/model/model_writelock.h"
#include "renderer_gui/renderer_gui_writelock.h"
#include "renderer_gui/renderer_gui_readlock.h"
#include "renderer_gui/renderer_gui.h"
#include "renderer_gui/renderer_gui_ref.h"
#include "../gfx/gui/gui_ref.h"
#include "../gfx/gui/gui_writelock.h"

namespace dragonpoop
{

    //ctor
    renderer::renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->g = (gfx_ref *)g->getRef();
        this->bDoRun = 1;
        this->bIsRun = 0;
        this->bActive = 1;
        this->bActiveOld = 0;
        this->gtsk = new renderer_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 3/*14*/, 1 );
        tp->addTask( this->tsk );
        this->fps = this->fthiss = 0;
    }

    //dtor
    renderer::~renderer( void )
    {
        this->_kill();
        delete this->tsk;
        delete this->gtsk;
        this->deleteModels();
        this->deleteGuis();
        delete this->g;
    }

    //stop task and deinit api
    void renderer::_kill( void )
    {
        volatile bool d;

        this->bDoRun = 0;
        d = this->bIsRun;
        while( d )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            d = this->bIsRun;
        }
    }

    //returns true if running
    bool renderer::isRunning( void )
    {
        if( !this->tsk )
            return 0;
        return this->bIsRun;
    }

    //return core
    core *renderer::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *renderer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_readlock( (renderer *)p, l );
    }

    //generate write lock
    shared_obj_writelock *renderer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_writelock( (renderer *)p, l );
    }

    //generate ref
    shared_obj_ref *renderer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_ref( (renderer *)p, k );
    }

    //stop renderer task
    void renderer::kill( void )
    {
        this->bDoRun = 0;
    }

    //run renderer from task
    void renderer::run( dptask_writelock *tskl, dpthread_lock *thd, renderer_writelock *r )
    {

        if( this->bIsRun )
        {
            if( !this->bDoRun )
            {
                this->deinitApi();
                this->bIsRun = 0;
                tskl->kill();
                return;
            }

            if( !this->runApi() )
                this->bDoRun = 0;
            else
            {
                this->runModels( thd, r );
                this->runGuis( thd, r );
                this->render( thd, r );
            }
            return;
        }

        if( !this->bDoRun )
        {
            tskl->kill();
            return;
        }

        if( this->initApi() )
            this->bIsRun = 1;
        else
            this->bDoRun = 0;
    }

    //render
    void renderer::render( dpthread_lock *thd, renderer_writelock *rl )
    {
        unsigned int w, h;
        uint64_t t, td;
        float f0;
        
        w = this->getWidth();
        h = this->getHeight();
        
        this->setViewport( w, h );
        this->clearScreen( 0.5f, 0.5f, 1.0f );
        this->prepareWorldRender( w, h );
        this->renderModels( thd, rl, 0, &this->m_world );
        
        this->prepareGuiRender( w, h );
        this->renderModels( thd, rl, 1, &this->m_gui );
        
        this->flipBuffer();
        
        this->fthiss += 1.0f;
        t = thd->getTicks();
        td = t - this->t_last_fps;
        if( td > 2000 )
        {
            f0 = (float)td / 1000.0f;
            this->fps = this->fthiss / f0;
            this->t_last_fps = t;
            this->fthiss = 0;
        }
    }
 
    //init graphics api
    bool renderer::initApi( void )
    {
        return 1;
    }

    //deinit graphics api
    void renderer::deinitApi( void )
    {
    }

    //do background graphics api processing
    bool renderer::runApi( void )
    {
        return 1;
    }

    //return screen/window width
    unsigned int renderer::getWidth( void )
    {
        return 1;
    }

    //return screen/window height
    unsigned int renderer::getHeight( void )
    {
        return 1;
    }

    //set viewport size
    void renderer::setViewport( unsigned int w, unsigned int h )
    {

    }

    //clear screen with color
    void renderer::clearScreen( float r, float g, float b )
    {

    }

    //prepare for rendering world
    void renderer::prepareWorldRender( unsigned int w, unsigned int h )
    {
        this->m_world.setPerspective( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 100.0f, 45.0f );
    }

    //prepare for rendering gui
    void renderer::prepareGuiRender( unsigned int w, unsigned int h )
    {
        this->m_gui.setOrtho( -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 10.0f );
    }

    //flip backbuffer and present scene to screen
    void renderer::flipBuffer( void )
    {

    }

    //run guis
    void renderer::runGuis( dpthread_lock *thd, renderer_writelock *rl )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        dpid_bytetree t;
        std::list<gui_ref *> li;
        std::list<gui_ref *>::iterator ii;
        gui_ref *pi;
        gui_writelock *pl;
        renderer_gui_writelock *ppl;
        shared_obj_guard o, og;
        gfx_readlock *gl;
        uint64_t tr;
        
        tr = thd->getTicks();
        if( tr - this->t_last_gui_ran < 200 )
            return;
        this->t_last_gui_ran = tr;
        
        gl = (gfx_readlock *)og.tryReadLock( this->g, 100, "renderer::runGuis" );
        if( !gl )
            return;
        
        //build id index
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            t.addLeaf( p->getId(), p );
        }
        
        //sync models and create if not exist
        gl->getGuis( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            pl = (gui_writelock *)o.tryWriteLock( pi, 100, "renderer::runGuis" );
            delete pi;
            if( !pl )
                continue;
            p = (renderer_gui *)t.findLeaf( pl->getId() );
            if( !p )
            {
                p = this->genGui( pl );
                if( p )
                    this->guis.push_back( p );
            }
            t.removeLeaf( p );
            if( !p )
                continue;
            ppl = (renderer_gui_writelock *)og.tryWriteLock( p, 100, "renderer::runModels" );
            if( !ppl )
                continue;
            ppl->run( thd, pl );
        }
        o.unlock();
        og.unlock();
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->guis.remove( p );
            delete p;
        }
        
    }
    
    //delete guis
    void renderer::deleteGuis( void )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        
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
    //MEMORY LEAK!
    //run models
    void renderer::runModels( dpthread_lock *thd, renderer_writelock *rl )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        dpid_bytetree t;
        std::list<model *> li;
        std::list<model *>::iterator ii;
        model *pi;
        model_writelock *pl;
        renderer_model_writelock *ppl;
        shared_obj_guard o, og;
        gfx_readlock *gl;
        uint64_t tr;
        
        tr = thd->getTicks();
        if( tr - this->t_last_m_ran < 200 )
            return;
        this->t_last_m_ran = tr;
        
        gl = (gfx_readlock *)og.tryReadLock( this->g, 100, "renderer::runModels" );
        if( !gl )
            return;
        
        //build id index
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            t.addLeaf( p->getId(), p );
        }
        
        //sync models and create if not exist
        gl->getModels( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            pl = (model_writelock *)o.tryWriteLock( pi, 100, "renderer::runModels" );
            delete pi;
            if( !pl )
                continue;
            p = (renderer_model *)t.findLeaf( pl->getId() );
            if( !p )
            {
                p = this->genModel( pl );
                if( p )
                    this->models.push_back( p );
            }
            t.removeLeaf( p );
            if( !p )
                continue;
            ppl = (renderer_model_writelock *)og.tryWriteLock( p, 100, "renderer::runModels" );
            if( !ppl )
                continue;
            ppl->run( thd, pl );
        }
        o.unlock();
        og.unlock();
        
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->models.remove( p );
            delete p;
        }
        
    }
    
    //render models
    void renderer::renderModels( dpthread_lock *thd, renderer_writelock *rl, bool doGui, dpmatrix *m_world )
    {
        std::list<renderer_model *> *l;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        renderer_model_readlock *pl;
        shared_obj_guard o;
        
        //render models
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_readlock *)o.tryReadLock( p, 30, "renderer::renderModels" );
            if( !pl )
                continue;
            pl->render( thd, rl, doGui, m_world );
        }
    }
    
    //delete models
    void renderer::deleteModels( void )
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

    //generate renderer model
    renderer_model *renderer::genModel( model_writelock *ml )
    {
        return new renderer_model( ml );
    }
    
    //generate renderer gui
    renderer_gui *renderer::genGui( gui_writelock *ml )
    {
        return new renderer_gui( ml );
    }
    
    //returns fps
    float renderer::getFps( void )
    {
        return this->fps;
    }

    //set active state
    void renderer::setActiveState( bool b )
    {
        this->bActive = b;
    }
    
};
