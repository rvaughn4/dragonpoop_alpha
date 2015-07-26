
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

namespace dragonpoop
{

    //ctor
    renderer::renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->g = (gfx_ref *)g->getRef();
        this->bDoRun = 1;
        this->bIsRun = 0;
        this->gtsk = new renderer_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 30, 1 );
        tp->addTask( this->tsk );
    }

    //dtor
    renderer::~renderer( void )
    {
        this->_kill();
        delete this->tsk;
        delete this->gtsk;
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
        unsigned int w, h;

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
                w = this->getWidth();
                h = this->getHeight();

                this->setViewport( w, h );
                this->clearScreen( 0.75f, 0.8f, 1.0f );
                this->prepareWorldRender( w, h );

                this->prepareGuiRender();
                this->flipBuffer();
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

    }

    //prepare for rendering gui
    void renderer::prepareGuiRender( void )
    {

    }

    //flip backbuffer and present scene to screen
    void renderer::flipBuffer( void )
    {

    }

};
