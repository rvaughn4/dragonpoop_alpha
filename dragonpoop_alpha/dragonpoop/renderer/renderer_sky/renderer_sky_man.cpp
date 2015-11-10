
#include "renderer_sky_man.h"
#include "renderer_sky_man_ref.h"
#include "renderer_sky_man_readlock.h"
#include "renderer_sky_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "renderer_sky_man_task.h"
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
#include "../../gfx/dpsky/dpsky_man.h"
#include "../../gfx/dpsky/dpsky_man_ref.h"
#include "../../gfx/dpsky/dpsky_man_readlock.h"
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
    renderer_sky_man::renderer_sky_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o, o1;
        gfx_writelock *gl;
        renderer_writelock *rl;
        renderer_commandlist_passer_writelock *cpl;
        
        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( clpasser, 5000, "renderer_sky_man::renderer_sky_man" );
        if( cpl )
            this->clpasser = (renderer_commandlist_passer_ref *)cpl->getRef();
        else
            this->clpasser = 0;
        o.unlock();
        
        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_sky_man::renderer_sky_man" );
        if( gl )
            gl->getSky( &this->g_skys );
        o.unlock();
        
        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        
        rl = (renderer_writelock *)o.writeLock( r, "renderer_sky_man::renderer_sky_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();
        
        this->thd = new dpthread_singletask( c->getMutexMaster(), 302 );
        this->_startTask( tp, 1 );
    }
    
    //dtor
    renderer_sky_man::~renderer_sky_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        this->deleteSky();
        o.unlock();
        this->unlink();
        delete this->thd;
        
        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_skys;
        delete this->tpr;
        delete this->clpasser;
        o.unlock();
    }
    
    //compute matrix
    void renderer_sky_man::computeMatrix( void )
    {
        this->m.setIdentity();
    }
    
    //return core
    core *renderer_sky_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *renderer_sky_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_sky_man_readlock( (renderer_sky_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_sky_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_sky_man_writelock( (renderer_sky_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_sky_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_sky_man_ref( (renderer_sky_man *)p, k );
    }
    
    //start task
    void renderer_sky_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        dpthread_lock *thdl;
        
        this->gtsk = new renderer_sky_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, "renderer_sky_man" );
        
        thdl = this->thd->lock();
        if( thdl )
        {
            thdl->addTask( this->tsk );
            delete thdl;
        }
    }
    
    //kill task
    void renderer_sky_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "renderer_sky_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void renderer_sky_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //sync skys
    void renderer_sky_man::sync( dpthread_lock *thd )
    {
        uint64_t t;
        
        t = thd->getTicks();
        if( t - this->t_last_sky_synced < 200 )
            return;
        this->t_last_sky_synced = t;
    }
    
    //run from manager thread
    void renderer_sky_man::run( dpthread_lock *thd, renderer_sky_man_writelock *l )
    {
        this->sync( thd );
        this->computeMatrix();
        this->runSky( thd );
        this->render( thd );
    }
    
    //delete skys
    void renderer_sky_man::deleteSky( void )
    {
    }
    
    //render into command list
    void renderer_sky_man::render( dpthread_lock *thd )
    {
        shared_obj_guard o, octxt, ocl;
        render_api_context_writelock *ctxl;
        render_api_commandlist_writelock *cll;
        renderer_commandlist_passer_writelock *cpl;
        shared_obj_guard ocpl;
        render_api_shader_ref *sdr;
        
        if( this->clpasser->t->sky_ready )
            return;
        
        cpl = (renderer_commandlist_passer_writelock *)ocpl.tryWriteLock( this->clpasser, 100, "renderer_sky_man::renderSky" );
        if( !cpl )
            return;
        
        cpl->setSky( this->clist );
        ocpl.unlock();
        
        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( this->ctx, 100, "renderer_sky_man::renderSky" );
        if( !ctxl )
            return;
        
        if( this->clist )
            delete this->clist;
        this->clist = ctxl->makeCmdList();
        if( !this->clist )
            return;
        
        cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->clist, 100, "renderer_sky_man::renderSky" );
        if( !cll )
            return;
        
        sdr = ctxl->makeShader( "sky" );
        if( !sdr )
            return;
        /*
        dpmatrix m1, m2;
        static float rr;
        rr += 1.0f;
        
        cll->setShader( sdr );
        cll->setAlpha( 0.5f );
        cll->setColor( 1.0f, 1.0f, 1.0f );
        cll->setMatrix( &this->m );
        cll->setTexture( this->sky.tex_stars, 0 );
        cll->setTexture( 0, 1 );
        cll->setIndexBuffer( this->sky.ib );
        cll->setVertexBuffer( this->sky.vb );
        cll->draw();
        
        m1.setIdentity();
        //m1.translate( 0, 0, -5.0f );
        m1.rotateZ( rr );
        m2.copy( &this->m );
        m2.multiply( &m1 );
        
        cll->setShader( sdr );
        cll->setAlpha( 1.0f );
        cll->setColor( 1.0f, 1.0f, 0.8f );
        cll->setMatrix( &m2 );
        cll->setTexture( this->sky.tex_sun, 0 );
        cll->setTexture( 0, 1 );
        cll->setIndexBuffer( this->sky.ib );
        cll->setVertexBuffer( this->sky.vb );
        cll->draw();
        
        cll->setShader( sdr );
        cll->setColor( 0.3f, 0.2f, 0.0f );
        cll->setAlpha( 1.0f );
        cll->setMatrix( &this->m );
        cll->setTexture( this->sky.tex_bg, 0 );
        cll->setTexture( 0, 1 );
        cll->setIndexBuffer( this->sky.ib );
        cll->setVertexBuffer( this->sky.vb );
        cll->draw();
        */

        if( cll->compile( ctxl ) )
            this->clpasser->t->sky_ready = 1;
        
        delete sdr;
    }
    
    //generate renderer sky
    //run sky
    void renderer_sky_man::runSky( dpthread_lock *thd )
    {
    }
    
    
};
