
#include "renderer.h"
#include "../core/core.h"
#include "../core/dptask/dptasks.h"
#include "renderer_task.h"
#include "renderer_readlock.h"
#include "renderer_writelock.h"
#include "renderer_ref.h"
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

#include "openglx_1o5_renderer/openglx_1o5_renderer_factory.h"
#include <thread>
#include <random>

namespace dragonpoop
{

    //ctor
    renderer::renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->tp = (dptaskpool_ref *)tp->getRef();
        this->g = (gfx_ref *)g->getRef();
        this->bDoRun = 1;
        this->bIsRun = 0;
        this->bActive = 1;
        this->bActiveOld = 0;
        this->ms_each_frame = 30;
        g->getModels( &this->m );
        this->gtsk = new renderer_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 3/*14*/, 1, "renderer" );
        tp->addTask( this->tsk );
        this->fps = this->fthiss = 0;
        g->getCameraPosition( &this->cam_pos );
        this->bCamSync = 0;
        this->rgui_mgr = 0;
        this->cs = new renderer_state_init_api( this );
    }

    //dtor
    renderer::~renderer( void )
    {
        shared_obj_guard o;

        this->_kill();

        o.tryWriteLock( this, 3000, "renderer::~renderer" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 3000, "renderer::~renderer" );
        delete this->tsk;
        delete this->gtsk;
        delete this->cs;
        this->deleteModels();
        delete this->m;
        delete this->g;
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
        renderer_gui_man::runFromRenderer( thd, this->rgui_mgr );
        this->runModels( thd, rl );
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
        return 1;
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
        this->deleteModels();
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
        this->deleteModels();
    }
    
    //render
    void renderer::render( dpthread_lock *thd, renderer_writelock *rl )
    {
        unsigned int w, h;
        uint64_t t, td;
        float f0;
        shared_obj_guard o;
        renderer_gui_man_readlock *gwl;
        
        w = this->getWidth();
        h = this->getHeight();
        
        this->setViewport( w, h );
        this->clearScreen( 0.5f, 0.5f, 1.0f );

        
        this->prepareWorldRender( w, h );
        this->renderModels( thd, rl, 0, &this->m_world );
        
        this->prepareGuiRender( w, h );
        gwl = (renderer_gui_man_readlock *)o.tryReadLock( this->rgui_mgr, 100, "renderer::render" );
        if( gwl )
            gwl->renderGuis( thd, rl, &this->m_gui );
        o.unlock();
        
        this->flipBuffer();
        
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
        return 1;
    }

    //deinit graphics api
    void renderer::deinitApi( void )
    {
    }

    //do background graphics api processing
    bool renderer::runApi( renderer_writelock *r, dpthread_lock *thd )
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

    //flip backbuffer and present scene to screen
    void renderer::flipBuffer( void )
    {

    }

    //run models
    void renderer::runModels( dpthread_lock *thd, renderer_writelock *rl )
    {
        std::list<renderer_model *> *l, d;
        std::list<renderer_model *>::iterator i;
        renderer_model *p;
        dpid_btree t;
        std::list<model *> li;
        std::list<model *>::iterator ii;
        model *pi;
        model_writelock *pl;
        renderer_model_writelock *ppl;
        shared_obj_guard o, og;
        model_man_readlock *ml;
        uint64_t tr;
        bool bDidFail;

        tr = thd->getTicks();
        if( tr - this->t_last_m_ran < 10 )
            return;
        this->t_last_m_ran = tr;
        
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            ppl = (renderer_model_writelock *)o.tryWriteLock( p, 30, "renderer::runModels" );
            if( !ppl )
                continue;
            ppl->run( thd );
        }
        
        if( tr - this->t_last_m_synced < 500 )
            return;
        this->t_last_m_synced = tr;
        
        ml = (model_man_readlock *)og.tryReadLock( this->m, 30, "renderer::runModels" );
        if( !ml )
            return;
        
        //build id index
        bDidFail = 0;
        l = &this->models;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            t.addLeaf( p->getId(), p );
        }
        
        //sync models and create if not exist
        ml->getModels( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            pl = (model_writelock *)o.tryWriteLock( pi, 300, "renderer::runModels" );
            if( !pl )
            {
                bDidFail = 1;
                continue;
            }
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
            ppl = (renderer_model_writelock *)og.tryWriteLock( p, 300, "renderer::runModels" );
            if( !ppl )
                continue;
        }
        o.unlock();
        og.unlock();
        
        if( bDidFail )
            return;
        
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
            pl = (renderer_model_readlock *)o.tryReadLock( p, 100, "renderer::renderModels" );
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
    renderer_gui_man *renderer::genGuiMan(  dptaskpool_writelock *tp )
    {
        return 0;
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
    void renderer::processMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb )
    {
        float w, h;
        renderer_gui_man_writelock *l;
        shared_obj_guard o;
        dpxyz_f t;
        
        w = this->getWidth();
        h = this->getHeight();
        x = x / w;
        y = y / h;
        
        x = ( 2.0f * x ) - 1.0f;
        y = ( 2.0f * y ) - 1.0f;
        y = -y;
        
        t.x = x;
        t.y = y;
        t.z = 0;
        this->m_gui_undo.transform( &t );
        
        l = (renderer_gui_man_writelock *)o.tryWriteLock( this->rgui_mgr, 1000, "renderer::processMouseInput" );
        if( !l )
            return;
        
        if( l->processGuiMouseInput( t.x, t.y, lb, rb ) )
            return;
    }
    
    //process keyboard input
    void renderer::processKbInput( std::string *skey_name, bool isDown )
    {
        renderer_gui_man_writelock *l;
        shared_obj_guard o;

        l = (renderer_gui_man_writelock *)o.tryWriteLock( this->rgui_mgr, 1000, "renderer::processMouseInput" );
        if( !l )
            return;

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
        
        gl->addRenderer( new openglx_1o5_renderer_factory() );
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
    
};
