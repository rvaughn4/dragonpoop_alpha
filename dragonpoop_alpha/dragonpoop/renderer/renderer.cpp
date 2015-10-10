
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
        this->ms_each_frame = 30;
        this->gtsk = new renderer_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 3/*14*/, 1 );
        tp->addTask( this->tsk );
        this->fps = this->fthiss = 0;
    }

    //dtor
    renderer::~renderer( void )
    {
        shared_obj_guard o;
        shared_obj_writelock *l;

        this->_kill();

        delete this->tsk;
        delete this->gtsk;

        l = o.tryWriteLock( this, 3000, "renderer::~renderer" );
        this->deleteModels();
        this->deleteGuis();
        o.unlock();

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
                this->deleteModels();
                this->deleteGuis();
                this->deinitApi();
                this->bIsRun = 0;
                tskl->kill();
                return;
            }

            if( !this->runApi( r, thd ) )
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
        //this->renderModels( thd, rl, 1, &this->m_gui );
        this->renderGuis( thd, rl, &this->m_gui );
        
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
        dpid nid;
        dpmatrix mat;
        
        //run guis
        tr = thd->getTicks();
        if( tr - this->t_last_gui_ran < 40 )
            return;
        this->t_last_gui_ran = tr;

        nid = dpid_null();
        mat.setTranslation( 0, 0, -1 );
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->compareParentId( nid ) )
                continue;
            ppl = (renderer_gui_writelock *)o.tryWriteLock( p, 10, "renderer::runGuis" );
            if( !ppl )
                continue;
            ppl->redoMatrix( thd, rl, &mat );
        }

        //make new guis and delete old
        if( tr - this->t_last_gui_synced < 250 )
            return;
        this->t_last_gui_synced = tr;
        
        gl = (gfx_readlock *)og.tryReadLock( this->g, 30, "renderer::runGuis" );
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
        this->focus_gui = nid;
        gl->getGuis( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            pl = (gui_writelock *)o.tryWriteLock( pi, 300, "renderer::runGuis" );
            if( !pl )
                continue;
            p = (renderer_gui *)t.findLeaf( pl->getId() );
            if( !p )
            {
                p = this->genGui( pl );
                if( p )
                    this->guis.push_back( p );
                pl->setRenderer( p );
            }
            t.removeLeaf( p );
            if( !p )
                continue;
            ppl = (renderer_gui_writelock *)og.tryWriteLock( p, 300, "renderer::runGuis" );
            if( !ppl )
                continue;
            ppl->run( thd );
            if( ppl->hasFocus() )
            {
                if( !ppl->getFocusChild( rl, &this->focus_gui ) )
                    this->focus_gui = ppl->getId();
            }
        }
        o.unlock();
        og.unlock();
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
            {
                ppl = (renderer_gui_writelock *)o.tryWriteLock( p, 10, "renderer::runGuis" );
                if( ppl )
                {
                    ppl->kill();
                    if( !ppl->isAlive() )
                        d.push_back( p );
                }
            }
        }
        o.unlock();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->guis.remove( p );
            delete p;
        }
        
    }
    
    //render guis
    void renderer::renderGuis( dpthread_lock *thd, renderer_writelock *rl, dpmatrix *m_world )
    {
        std::list<renderer_gui *> *l, lz, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_readlock *pl;
        shared_obj_guard o;
        dpid nid;
        int max_z, z;
        
        l = &this->guis;
        nid = dpid_null();
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( p->compareParentId( nid ) )
                lz.push_back( p );
        }
        
        max_z = 0;
        l = &lz;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            z = p->getZ();
            if( z > max_z )
                max_z = z;
        }
        max_z++;
        
        for( z = max_z; z >= 0; z-- )
        {
            l = &lz;
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( z != p->getZ() )
                    continue;
                pl = (renderer_gui_readlock *)o.tryReadLock( p, 100, "renderer::renderGuis" );
                if( !pl )
                    continue;
                pl->render( thd, rl, m_world );
                d.push_back( p );
            }
            
            l = &d;
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                lz.remove( p );
            }
            d.clear();
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
        gfx_readlock *gl;
        uint64_t tr;

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
        
        gl = (gfx_readlock *)og.tryReadLock( this->g, 30, "renderer::runModels" );
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
            pl = (model_writelock *)o.tryWriteLock( pi, 300, "renderer::runModels" );
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
            ppl = (renderer_model_writelock *)og.tryWriteLock( p, 300, "renderer::runModels" );
            if( !ppl )
                continue;
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
    renderer_gui *renderer::genGui( gui_writelock *ml )
    {
        return new renderer_gui( ml );
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
    
    //return guis
    void renderer::getChildrenGuis( std::list<renderer_gui *> *ll, dpid pid )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( p->compareParentId( pid ) )
                ll->push_back( p );
        }
    }

    //process mouse input
    void renderer::processMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb )
    {
        float w, h;
        
        w = this->getWidth();
        h = this->getHeight();
        x = x / w;
        y = y / h;
        
        x = ( 2.0f * x ) - 1.0f;
        y = ( 2.0f * y ) - 1.0f;
        y = -y;
        
        if( this->processGuiMouseInput( r, x, y, lb, rb ) )
            return;
    }
    
    //process mouse input
    bool renderer::processGuiMouseInput( renderer_writelock *r, float x, float y, bool lb, bool rb )
    {
        std::list<renderer_gui *> *l, lz, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        dpid nid;
        int max_z, z;
        dpxyz_f t;
        
        t.x = x;
        t.y = y;
        t.z = 0;
        this->m_gui_undo.transform( &t );
        
        l = &this->guis;
        nid = dpid_null();
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( p->compareParentId( nid ) )
                lz.push_back( p );
        }
        
        max_z = 0;
        l = &lz;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            z = p->getZ();
            if( z > max_z )
                max_z = z;
        }
        max_z++;

        this->hover_gui = dpid_null();
        for( z = 0; z < max_z; z++ )
        {
            l = &lz;
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( z != p->getZ() )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer::processGuiMouseInput" );
                if( !pl )
                    continue;
                if( pl->processMouse( r, t.x, t.y, lb, rb ) )
                {
                    this->hover_gui = pl->getHoverId();
                    return 1;
                }
                d.push_back( p );
            }
            
            l = &d;
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                lz.remove( p );
            }
            d.clear();
        }
        
        return 0;
    }
    
    //get hovering gui id
    dpid renderer::getHoverId( void )
    {
        return this->hover_gui;
    }
    
    //process keyboard input
    void renderer::processKbInput( std::string *skey_name, bool isDown )
    {
        this->processGuiKbInput( skey_name, isDown );
    }
    
    //process gui keyboard input
    void renderer::processGuiKbInput( std::string *skey_name, bool isDown )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer::processGuiKbInput" );
            if( !pl )
                return;
            if( !pl->compareId( this->focus_gui ) )
                continue;
            pl->processKb( skey_name, isDown );
            return;
        }
    }
    
    //gets selected text from gui (copy or cut)
    bool renderer::getSelectedText( std::string *s, bool bDoCut )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer::getSelectedText" );
            if( !pl )
                return 0;
            if( !pl->compareId( this->focus_gui ) )
                continue;
            pl->getSelectedText( s, bDoCut );
            return 1;
        }
        
        return 0;
    }
    
    //sets selected text in gui (paste)
    bool renderer::setSelectedText( std::string *s )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer::setSelectedText" );
            if( !pl )
                return 0;
            if( !pl->compareId( this->focus_gui ) )
                continue;
            pl->setSelectedText( s );
            return 1;
        }
        
        return 0;
    }
    
};
