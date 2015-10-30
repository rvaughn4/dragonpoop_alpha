
#include "renderer.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "renderer_task.h"
#include "renderer_readlock.h"
#include "renderer_writelock.h"
#include "renderer_ref.h"
#include "../core/dpthread/dpthread_singletask.h"
#include "../core/dpthread/dpthread_lock.h"
#include "../core/dptaskpool/dptaskpool_writelock.h"
#include "../core/dptaskpool/dptaskpool_ref.h"
#include "../core/shared_obj/shared_obj_guard.h"
#include "../gfx/gfx_writelock.h"
#include "../gfx/gfx_readlock.h"
#include "../gfx/gfx_ref.h"
#include "../gfx/gfx.h"
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
#include "../gfx/model/model_man_ref.h"
#include "../gfx/model/model_man_readlock.h"
#include "../gfx/gui/gui_man_ref.h"
#include "../gfx/gui/gui_man_readlock.h"
#include "renderer_state.h"
#include "renderer_state_init_api.h"
#include "renderer_gui/renderer_gui_man.h"
#include "renderer_gui/renderer_gui_man_writelock.h"
#include "renderer_gui/renderer_gui_man_readlock.h"
#include "renderer_model/renderer_model_man.h"
#include "renderer_model/renderer_model_man_writelock.h"
#include "renderer_model/renderer_model_man_readlock.h"
#include "api_stuff/render_api/render_api.h"
#include "api_stuff/render_api/render_api_writelock.h"
#include "api_stuff/render_api/render_api_context_ref.h"
#include "api_stuff/render_api/render_api_context_writelock.h"
#include "api_stuff/render_api/render_api_commandlist_ref.h"
#include "api_stuff/render_api/render_api_commandlist_writelock.h"
#include "renderer_factory.h"
#include "x11_opengl_1o5_renderer/x11_opengl_1o5_renderer_factory.h"
#include "renderer_commandlist_passer.h"
#include "renderer_commandlist_passer_ref.h"
#include "renderer_commandlist_passer_writelock.h"

#include <thread>
#include <random>

namespace dragonpoop
{

    //ctor
    renderer::renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        dpthread_lock *thdl;
        
        this->clpasser = new renderer_commandlist_passer( c->getMutexMaster() );
        this->api = 0;
        this->main_ctx = 0;
        this->c = c;
        this->tp = (dptaskpool_ref *)tp->getRef();
        this->g = (gfx_ref *)g->getRef();
        this->bDoRun = 1;
        this->bIsRun = 0;
        this->bActive = 1;
        this->bActiveOld = 0;
        this->ms_each_frame = 30;
        this->thd = new dpthread_singletask( c->getMutexMaster(), 301 );
        this->gtsk = new renderer_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 10, 1, "renderer" );
        thdl = this->thd->lock();
        if( thdl )
        {
            thdl->addTask( this->tsk );
            delete thdl;
        }
        this->fps = this->fthiss = 0;
        g->getCameraPosition( &this->cam_pos );
        this->bCamSync = 0;
        this->rgui_mgr = 0;
        this->rmodel_mgr = 0;
        this->cs = new renderer_state_init_api( this );
        this->gui_cl = 0;
        this->new_gui_cl = 0;
        this->model_cl = 0;
        this->new_model_cl = 0;
    }

    //dtor
    renderer::~renderer( void )
    {
        shared_obj_guard o;
        render_api_commandlist_ref *cl;

        this->_kill();

        o.tryWriteLock( this, 3000, "renderer::~renderer" );
        o.unlock();
        this->unlink();
        delete this->thd;

        o.tryWriteLock( this, 3000, "renderer::~renderer" );
        cl = this->new_gui_cl;
        if( cl )
            delete cl;
        if( this->gui_cl )
            delete this->gui_cl;
        cl = this->new_model_cl;
        if( cl )
            delete cl;
        if( this->model_cl )
            delete this->model_cl;
        delete this->tsk;
        delete this->gtsk;
        delete this->cs;
        delete this->g;
        delete this->clpasser;
        delete this->tp;
        o.unlock();
    }

    //stop task and deinit api
    void renderer::_kill( void )
    {
        volatile bool d;

        this->bDoRun = 0;
        d = this->bIsRun;
        while( d )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
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
        this->_kill();
    }

    //run renderer from task
    void renderer::run( dptask_writelock *tskl, dpthread_lock *thd, renderer_writelock *r )
    {
        renderer_state *ns;
        
        ns = this->cs->run( this, r, thd );
        if( ns )
        {
            delete this->cs;
            this->cs = ns;
        }
    }

    //run renderer
    void renderer::state_run( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->runApi( rl, thd );
        this->_syncCam();
        this->render( thd, rl );
    }
    
    //init api
    bool renderer::state_initApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        return this->initApi();
    }
    
    //init gui manager
    bool renderer::state_initGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        dptaskpool_writelock *tp;
        shared_obj_guard o;
        
        tp = (dptaskpool_writelock *)o.tryWriteLock( this->tp, 1000, "renderer::state_initGui" );
        if( !tp )
            return 0;
        
        this->rgui_mgr = this->genGuiMan( tp );
        return this->rgui_mgr != 0;
    }
    
    //init model manager
    bool renderer::state_initModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        dptaskpool_writelock *tp;
        shared_obj_guard o;
        
        tp = (dptaskpool_writelock *)o.tryWriteLock( this->tp, 1000, "renderer::state_initGui" );
        if( !tp )
            return 0;
        
        this->rmodel_mgr = this->genModelMan( tp );
        return this->rmodel_mgr != 0;
    }
    
    //start api
    void renderer::state_startApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        
    }
    
    //start gui manager
    void renderer::state_startGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        renderer_gui_man_writelock *l;
        shared_obj_guard o;
        
        l = (renderer_gui_man_writelock *)o.tryWriteLock( this->rgui_mgr, 3000, "renderer::state_startGui" );
        if( !l )
            return;
        //l->start();
        
        
    }
    
    //start model manager
    void renderer::state_startModel( dpthread_lock *thd, renderer_writelock *rl )
    {
    }
    
    //stop api
    void renderer::state_stopApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        
    }
    
    //stop gui manager
    void renderer::state_stopGui( dpthread_lock *thd, renderer_writelock *rl )
    {
    }
    
    //stop model manager
    void renderer::state_stopModel( dpthread_lock *thd, renderer_writelock *rl )
    {
    }
    
    //deinit api
    void renderer::state_deinitApi( dpthread_lock *thd, renderer_writelock *rl )
    {
        this->deinitApi();
    }
    
    //init gui manager
    void renderer::state_deinitGui( dpthread_lock *thd, renderer_writelock *rl )
    {
        delete this->rgui_mgr;
        this->rgui_mgr = 0;
    }
    
    //init model manager
    void renderer::state_deinitModel( dpthread_lock *thd, renderer_writelock *rl )
    {
        delete this->rmodel_mgr;
        this->rmodel_mgr = 0;
    }
    
    //render
    void renderer::render( dpthread_lock *thd, renderer_writelock *rl )
    {
        unsigned int w, h;
        uint64_t t, td;
        float f0;
        shared_obj_guard o, octx, ocl;
        render_api_context_writelock *ctxl;
        render_api_writelock *al;
        render_api_commandlist_writelock *cll;
        renderer_commandlist_passer_writelock *cpl;
        
        if( this->new_model_cl )
        {
            if( this->model_cl )
                delete this->model_cl;
            this->model_cl = this->new_model_cl;
            this->new_model_cl = 0;
        }
        if( this->new_gui_cl )
        {
            if( this->gui_cl )
                delete this->gui_cl;
            this->gui_cl = this->new_gui_cl;
            this->new_gui_cl = 0;
        }
        
        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( this->clpasser, 3, "renderer::render" );
        if( cpl )
        {
            this->new_gui_cl = cpl->getGui();
            this->new_model_cl = cpl->getModel();
            cpl->setPosition( &this->cam_pos );
            if( !this->new_model_cl )
                return;
        }
        o.unlock();
        
        al = (render_api_writelock *)octx.tryWriteLock( this->api, 30, "renderer::render" );
        if( !al )
            return;
        w = al->getWidth();
        h = al->getHeight();

        ctxl = (render_api_context_writelock *)octx.tryWriteLock( this->main_ctx, 30, "renderer::render" );
        if( !ctxl )
            return;
        
        ctxl->makeActive();
        ctxl->setViewport( w, h );
        ctxl->clearColor( 0.5f, 0.5f, 1.0f );
        ctxl->clearDepth( 1.0f );
        
        if( this->model_cl )
        {
            cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->model_cl, 30, "renderer::render" );
            if( cll )
                cll->execute( ctxl );
            ocl.unlock();
        }
        
        ctxl->clearDepth( 1.0f );
        if( this->gui_cl )
        {
            cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->gui_cl, 3, "renderer::render" );
            if( cll )
                cll->execute( ctxl );
            ocl.unlock();
        }
        
        ctxl->flipBackBuffer();
        
        this->fthiss += 1.0f;
        t = thd->getTicks();
        td = t - this->t_last_fps;
        if( td > 2000 )
        {
            f0 = (float)td / 1000.0f;
            this->fps = this->fthiss / f0;
            this->t_last_fps = t;
            if( this->fps > 0 )
                f0 = 1000.0f / this->fps;
            this->ms_each_frame = (uint64_t)f0;
            this->fthiss = 0;
        }

    }
 
    //init graphics api
    bool renderer::initApi( void )
    {
        render_api_writelock *al;
        shared_obj_guard o;

        if( this->main_ctx )
            delete this->main_ctx;
        this->main_ctx = 0;
        if( this->api )
            delete this->api;
        
        this->api = this->genRenderApi( this->c->getMutexMaster() );
        if( !this->api )
            return 0;
        
        al = (render_api_writelock *)o.tryWriteLock( this->api, 100, "renderer::runApi" );
        if( !al )
            return 0;

        this->main_ctx = al->getContext();
        return this->main_ctx != 0;
    }

    //deinit graphics api
    void renderer::deinitApi( void )
    {
        if( this->main_ctx )
            delete this->main_ctx;
        this->main_ctx = 0;
        if( this->api )
            delete this->api;
        this->api = 0;
    }

    //do background graphics api processing
    bool renderer::runApi( renderer_writelock *r, dpthread_lock *thd )
    {
        render_api_writelock *al;
        shared_obj_guard o, o1;
        window_mouse_input m;
        window_kb_input k;
        renderer_gui_man_writelock *gl;
        uint64_t t;
        
        al = (render_api_writelock *)o.tryWriteLock( this->api, 100, "renderer::runApi" );
        if( !al )
            return 1;
            
        al->run();
        if( !al->isOpen() )
            this->c->kill();
        
        
        t = thd->getTicks();
        if( t - this->t_last_input < 100 )
            return 1;
        this->t_last_input = t;
        if( !al->hasKBInput() && !al->hasMouseInput() )
            return 1;
        
        gl = (renderer_gui_man_writelock *)o1.tryWriteLock( this->rgui_mgr, 100, "renderer::runApi" );
        if( !gl )
            return 1;
        
        if( al->hasMouseInput() )
        {
            while( al->getMouseInput( &m ) )
                this->processMouseInput( gl, al, m.x, m.y, m.lb, m.rb );
        }
        if( al->hasKBInput() )
        {
            while( al->getKBInput( &k ) )
                this->processKbInput( gl, &k.sname, k.bDown );
        }
        
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

    //prepare for rendering world
    void renderer::prepareWorldRender( unsigned int w, unsigned int h )
    {
        float sw, sh, rw, rh, r, dw, dh;
        
        sw = 1920.0f;
        sh = 1080.0f;
        
        rw = sw / w;
        rh = sh / h;
        
        r = rw;
        if( r < rh )
            r = rh;
        dw = r - rw;
        dh = r - rh;
        
        this->m_world.setPerspective( -r - dw, -r - dh, 1.0f, r + dw, r + dh, 100.0f, 45.0f );
    }

    //prepare for rendering gui
    void renderer::prepareGuiRender( unsigned int w, unsigned int h )
    {
        float sw, sh, rw, rh, r, dw, dh, ss;
        
        sw = 1920.0f;
        sh = 1080.0f;
        
        ss = sw * sw + sh * sh;
        ss = sqrtf( ss );
        
        rw = sw / w;
        rh = sh / h;
        
        r = rw;
        if( r < rh )
            r = rh;
        w = w * r;
        h = h * r;
        dw = w - sw;
        dh = h - sh;
        dw *= 0.5f;
        dh *= 0.5f;
        
        this->m_gui.setOrtho( -dw, sh + dh, 0.0f, sw + dw, -dh, ss );
        this->m_gui_undo.inverse( &this->m_gui );
    }

    //generate renderer model
    renderer_model_man *renderer::genModelMan( dptaskpool_writelock *tp )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        render_api_context_ref *ctx;
        
        l = (render_api_writelock *)o.tryWriteLock( this->api, 5000, "renderer::genModelMan" );
        if( !l )
            return 0;
        ctx = l->getContext();
        if( !ctx )
            return 0;
        
        return new renderer_model_man( this->c, this, tp, ctx, this->clpasser, 1920, 1080 );
    }
    
    //generate renderer gui
    renderer_gui_man *renderer::genGuiMan( dptaskpool_writelock *tp )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        render_api_context_ref *ctx;
        
        l = (render_api_writelock *)o.tryWriteLock( this->api, 5000, "renderer::genGuiMan" );
        if( !l )
            return 0;
        ctx = l->getContext();
        if( !ctx )
            return 0;
        
        return new renderer_gui_man( this->c, this, tp, ctx, this->clpasser, 1920, 1080 );
    }
    
    //returns fps
    float renderer::getFps( void )
    {
        return this->fps;
    }

    //return ms each frame
    unsigned int renderer::getMsPerFrame( void )
    {
        return (unsigned int)this->ms_each_frame;
    }
    
    //get renderer name
    void renderer::getName( std::string *s )
    {
        
    }
    
    //get texture memory used
    unsigned int renderer::getTextureMemory( void )
    {
        return 0;
    }
    
    //get vertex memory used
    unsigned int renderer::getVertexMemory( void )
    {
        return 0;
    }

    //process mouse input
    void renderer::processMouseInput( renderer_gui_man_writelock *l, render_api_writelock *al, float x, float y, bool lb, bool rb )
    {
        float w, h;

        w = al->getWidth();
        h = al->getHeight();

        l->processGuiMouseInput( w, h, x, y, lb, rb );
    }
    
    //process keyboard input
    void renderer::processKbInput( renderer_gui_man_writelock *l, std::string *skey_name, bool isDown )
    {
        l->processGuiKbInput( skey_name, isDown );
    }
    
    //gets camera position
    void renderer::getCameraPosition( dpposition *p )
    {
        p->copy( &this->cam_pos );
    }
    
    //sync camera position
    void renderer::syncCamera( void )
    {
        this->bCamSync = 1;
    }
    
    //get position difference from camera
    void renderer::getPositionRelativeToCamera( dpposition *in_obj_pos, uint64_t t_epoch_now, dpxyz_f *out_xyz )
    {
        this->cam_pos.getDifference( in_obj_pos, t_epoch_now, out_xyz );
    }
    
    //sync camera
    void renderer::_syncCam( void )
    {
        shared_obj_guard o;
        gfx_readlock *gl;
        
        if( !this->bCamSync )
            return;
        gl = (gfx_readlock *)o.tryReadLock( this->g, 100, "renderer::_syncCam" );
        if( !gl )
            return;
        
        this->bCamSync = 0;
        gl->getCameraPosition( &this->cam_pos );
    }
    
    //populate renderer list
    void renderer::addRenderers( gfx *g )
    {
        gfx_writelock *gl;
        shared_obj_guard o;
        
        gl = (gfx_writelock *)o.tryWriteLock( g, 5000, "renderer::addRenderers" );
        if( !gl )
            return;
        
        gl->addRenderer( new x11_opengl_1o5_renderer_factory( 1 ) );
    }
    
    //gets selected text from gui (copy or cut)
    bool renderer::getSelectedText( std::string *s, bool bDoCut )
    {
        shared_obj_guard o;
        renderer_gui_man_writelock *l;
        
        l = (renderer_gui_man_writelock *)o.tryWriteLock( this->rgui_mgr, 100, "renderer::getSelectedText" );
        if( !l )
            return 0;
        
        return l->getSelectedText( s, bDoCut );
    }
    
    //sets selected text in gui (paste)
    bool renderer::setSelectedText( std::string *s )
    {
        shared_obj_guard o;
        renderer_gui_man_writelock *l;
        
        l = (renderer_gui_man_writelock *)o.tryWriteLock( this->rgui_mgr, 100, "renderer::setSelectedText" );
        if( !l )
            return 0;
        
        return l->setSelectedText( s );
    }
    
    //generate render api
    render_api *renderer::genRenderApi( dpmutex_master *mm )
    {
        return 0;
    }
    
    //returns true if gui commandlist is set
    bool renderer::isGuiCommandListUploaded( void )
    {
        render_api_commandlist_ref *cl;
        cl = this->new_gui_cl;
        return cl != 0;
    }
    
    //set gui commandlist
    void renderer::uploadGuiCommandList( render_api_commandlist_ref *cl )
    {
        render_api_commandlist_ref *ocl;
        ocl = this->new_gui_cl;
        if( ocl )
            delete ocl;
        this->new_gui_cl = cl;
    }
    
    //returns true if model commandlist is set
    bool renderer::isModelCommandListUploaded( void )
    {
        render_api_commandlist_ref *cl;
        cl = this->new_model_cl;
        return cl != 0;
    }
    
    //set model commandlist
    void renderer::uploadModelCommandList( render_api_commandlist_ref *cl )
    {
        render_api_commandlist_ref *ocl;
        ocl = this->new_model_cl;
        if( ocl )
            delete ocl;
        this->new_model_cl = cl;
    }
    
};
