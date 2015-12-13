
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
#include "../../gfx/dpsky/dpsky.h"
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
#include <string.h>

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
        memset( &this->stuff, 0, sizeof( this->stuff ) );

        rl = (renderer_writelock *)o.writeLock( r, "renderer_sky_man::renderer_sky_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();

        this->_startTask( tp, 5 );
    }

    //dtor
    renderer_sky_man::~renderer_sky_man( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "renderer_sky_man::~renderer_sky_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        this->deleteSky();
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
        this->gtsk = new renderer_sky_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, 1, "renderer_sky_man" );
        tp->addTask( this->tsk );
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
        shared_obj_guard o;
        dpsky_man_readlock *l;
        dpsky_stuff *s;


        t = thd->getTicks();
        if( t - this->t_last_sky_synced < 200 )
            return;
        this->t_last_sky_synced = t;


        l = (dpsky_man_readlock *)o.tryReadLock( this->g_skys, 100, "renderer_sky_man::runSky" );
        if( !l )
            return;
        s = l->getSky();

        this->stuff.sky_time = s->sky_time;
    }

    //run from manager thread
    void renderer_sky_man::run( dpthread_lock *thd, renderer_sky_man_writelock *l )
    {
        this->computeMatrix();
        this->runSky( thd );
        this->render( thd );
        this->sync( thd );
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
        dpmatrix m1, m2;
        float abs_sky_time, abs_smooth_sky_time, daylight;

        if( renderer_commandlist_passer::waitForFlag( &this->clpasser->t->sky_ready, 0, 3 ) )
            return;

        cpl = (renderer_commandlist_passer_writelock *)ocpl.tryWriteLock( this->clpasser, 100, "renderer_sky_man::renderSky" );
        if( !cpl )
            return;

        cpl->setSky( this->clist );
        ocpl.unlock();

        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( this->ctx, 100, "renderer_sky_man::renderSky" );
        if( !ctxl )
            return;

        ctxl->makeActive( thd );
        ctxl->run();
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

        abs_sky_time = this->stuff.sky_time;
        if( abs_sky_time < 0 )
            abs_sky_time = -abs_sky_time;
        abs_smooth_sky_time = this->stuff.smooth_sky_time;
        if( abs_smooth_sky_time < 0 )
            abs_smooth_sky_time = -abs_smooth_sky_time;
        abs_smooth_sky_time += ( abs_sky_time - abs_smooth_sky_time ) * 0.3f;
        if( this->stuff.sky_time < 0 )
            this->stuff.smooth_sky_time = -abs_smooth_sky_time;
        else
            this->stuff.smooth_sky_time = abs_smooth_sky_time;
        if( this->stuff.smooth_sky_time > 0 )
        {
            if( abs_smooth_sky_time < 0.5f )
                daylight = ( 0.5f - abs_smooth_sky_time );
            else
                daylight = abs_smooth_sky_time - 0.5f;
        }
        else
        {
            if( abs_smooth_sky_time < 0.2f )
                daylight = abs_smooth_sky_time / 0.2f * 0.5f + 0.5f;
            else
            {
                if( abs_smooth_sky_time > 0.8f )
                    daylight = ( ( 1.0f - abs_smooth_sky_time ) / 0.2f ) * 0.5f + 0.5f;
                else
                    daylight = 1.0f;
            }
        }


        m1.setIdentity();
        //m1.translate( 0, 0, -5.0f );
       // m1.rotateZ( rr );
        m2.copy( &this->m );
        m2.multiply( &m1 );

        cll->setShader( sdr );
        cll->setMatrix( &m2 );
        cll->setTexture( 0, 1 );

        cll->setColor( 1, 1, 1 );
        cll->setAlpha( 1.0f - daylight );

        cll->setTexture( this->stuff.skyboxtex.stars.front, 0 );
        cll->setIndexBuffer( this->stuff.skybox.front.ib );
        cll->setVertexBuffer( this->stuff.skybox.front.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.stars.back, 0 );
        cll->setIndexBuffer( this->stuff.skybox.back.ib );
        cll->setVertexBuffer( this->stuff.skybox.back.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.stars.top, 0 );
        cll->setIndexBuffer( this->stuff.skybox.top.ib );
        cll->setVertexBuffer( this->stuff.skybox.top.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.stars.bottom, 0 );
        cll->setIndexBuffer( this->stuff.skybox.bottom.ib );
        cll->setVertexBuffer( this->stuff.skybox.bottom.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.stars.left, 0 );
        cll->setIndexBuffer( this->stuff.skybox.left.ib );
        cll->setVertexBuffer( this->stuff.skybox.left.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.stars.right, 0 );
        cll->setIndexBuffer( this->stuff.skybox.right.ib );
        cll->setVertexBuffer( this->stuff.skybox.right.vb );
        cll->draw();

        cll->setAlpha( 1.0f );
        cll->setColor( 0.85f * daylight, 0.9f * daylight, 0.9f * daylight + 0.1f );

        cll->setTexture( this->stuff.skyboxtex.mask.front, 0 );
        cll->setIndexBuffer( this->stuff.skybox.front.ib );
        cll->setVertexBuffer( this->stuff.skybox.front.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.mask.back, 0 );
        cll->setIndexBuffer( this->stuff.skybox.back.ib );
        cll->setVertexBuffer( this->stuff.skybox.back.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.mask.top, 0 );
        cll->setIndexBuffer( this->stuff.skybox.top.ib );
        cll->setVertexBuffer( this->stuff.skybox.top.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.mask.left, 0 );
        cll->setIndexBuffer( this->stuff.skybox.left.ib );
        cll->setVertexBuffer( this->stuff.skybox.left.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.mask.right, 0 );
        cll->setIndexBuffer( this->stuff.skybox.right.ib );
        cll->setVertexBuffer( this->stuff.skybox.right.vb );
        cll->draw();

        cll->setTexture( this->stuff.skyboxtex.mask.bottom, 0 );
        cll->setIndexBuffer( this->stuff.skybox.bottom.ib );
        cll->setVertexBuffer( this->stuff.skybox.bottom.vb );
        cll->draw();

        m1.setIdentity();
        //m1.translate( 0, 0, -5.0f );
        m1.rotateX( this->stuff.smooth_sky_time * -180.0f );
        m2.copy( &this->m );
        m2.multiply( &m1 );
        cll->setMatrix( &m2 );


        cll->setAlpha( 1.0f );
        cll->setColor( 0.5f + 0.5f * daylight, 0.25f + 0.75f * daylight, 0.1f + 0.75f * daylight );

        cll->setTexture( this->stuff.billboardtex.sun, 0 );
        cll->setIndexBuffer( this->stuff.billboard.ib );
        cll->setVertexBuffer( this->stuff.billboard.vb );
        cll->draw();

        if( cll->compile( ctxl ) )
            this->clpasser->t->sky_ready = 1;

        delete sdr;
    }

    //generate renderer sky
    //run sky
    void renderer_sky_man::runSky( dpthread_lock *thd )
    {
        dpsky_man_readlock *l;
        render_api_context_writelock *c;
        shared_obj_guard o1, o2;
        dpsky_stuff *s;

        if( this->stuff.skybox.front.vb )
            return;

        l = (dpsky_man_readlock *)o1.tryReadLock( this->g_skys, 100, "renderer_sky_man::runSky" );
        if( !l )
            return;
        c = (render_api_context_writelock *)o2.tryWriteLock( this->ctx, 100, "renderer_sky_man::runSky" );
        if( !c )
            return;

        s = l->getSky();
        c->makeActive( thd );

        this->stuff.skybox.front.vb = c->makeVertexBuffer( &s->skybox.front.vb );
        this->stuff.skybox.front.ib = c->makeIndexBuffer( &s->skybox.front.ib );
        this->stuff.skybox.back.vb = c->makeVertexBuffer( &s->skybox.back.vb );
        this->stuff.skybox.back.ib = c->makeIndexBuffer( &s->skybox.back.ib );
        this->stuff.skybox.top.vb = c->makeVertexBuffer( &s->skybox.top.vb );
        this->stuff.skybox.top.ib = c->makeIndexBuffer( &s->skybox.top.ib );
        this->stuff.skybox.bottom.vb = c->makeVertexBuffer( &s->skybox.bottom.vb );
        this->stuff.skybox.bottom.ib = c->makeIndexBuffer( &s->skybox.bottom.ib );
        this->stuff.skybox.left.vb = c->makeVertexBuffer( &s->skybox.left.vb );
        this->stuff.skybox.left.ib = c->makeIndexBuffer( &s->skybox.left.ib );
        this->stuff.skybox.right.vb = c->makeVertexBuffer( &s->skybox.right.vb );
        this->stuff.skybox.right.ib = c->makeIndexBuffer( &s->skybox.right.ib );

        this->stuff.billboard.vb = c->makeVertexBuffer( &s->billboard.vb );
        this->stuff.billboard.ib = c->makeIndexBuffer( &s->billboard.ib );

        this->stuff.skyboxtex.mask.front = c->makeTexture( &s->skyboxtex.mask.front );
        this->stuff.skyboxtex.mask.back = c->makeTexture( &s->skyboxtex.mask.back );
        this->stuff.skyboxtex.mask.top = c->makeTexture( &s->skyboxtex.mask.top );
        this->stuff.skyboxtex.mask.bottom = c->makeTexture( &s->skyboxtex.mask.bottom );
        this->stuff.skyboxtex.mask.left = c->makeTexture( &s->skyboxtex.mask.left );
        this->stuff.skyboxtex.mask.right = c->makeTexture( &s->skyboxtex.mask.right );

        this->stuff.skyboxtex.stars.front = c->makeTexture( &s->skyboxtex.stars.front );
        this->stuff.skyboxtex.stars.back = c->makeTexture( &s->skyboxtex.stars.back );
        this->stuff.skyboxtex.stars.top = c->makeTexture( &s->skyboxtex.stars.top );
        this->stuff.skyboxtex.stars.bottom = c->makeTexture( &s->skyboxtex.stars.bottom );
        this->stuff.skyboxtex.stars.left = c->makeTexture( &s->skyboxtex.stars.left );
        this->stuff.skyboxtex.stars.right = c->makeTexture( &s->skyboxtex.stars.right );

        this->stuff.billboardtex.sun = c->makeTexture( &s->billboardtex.sun );
        this->stuff.billboardtex.moon = c->makeTexture( &s->billboardtex.moon );
    }


};
