
#include "renderer_gui_man.h"
#include "renderer_gui_man_ref.h"
#include "renderer_gui_man_readlock.h"
#include "renderer_gui_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "renderer_gui_man_task.h"
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
#include "../../gfx/gui/gui_man_ref.h"
#include "../../gfx/gui/gui_man_readlock.h"
#include "renderer_gui.h"
#include "renderer_gui_writelock.h"
#include "renderer_gui_readlock.h"
#include "../../gfx/gui/gui_ref.h"
#include "../../gfx/gui/gui_readlock.h"
#include "../../gfx/gui/gui_writelock.h"
#include "../../core/dpbtree/dpid_btree.h"
#include "../api_stuff/render_api/render_api_shader_ref.h"
#include "../api_stuff/render_api/render_api_context_ref.h"
#include "../api_stuff/render_api/render_api_context_writelock.h"
#include "../api_stuff/render_api/render_api_context_readlock.h"
#include "../api_stuff/render_api/render_api_commandlist_ref.h"
#include "../api_stuff/render_api/render_api_commandlist_writelock.h"
#include "renderer_gui.h"

#include <math.h>
#include <thread>

namespace dragonpoop
{
    
    //ctor
    renderer_gui_man::renderer_gui_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, float log_screen_width, float log_screen_height ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        renderer_writelock *rl;
        render_api_context_writelock *cl;
        
        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        cl = (render_api_context_writelock *)o.writeLock( this->ctx, "renderer_gui_man::renderer_gui_man" );
        if( cl )
            this->sdr = cl->makeShader( "gui" );
        else
            this->sdr = 0;

        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_gui_man::renderer_gui_man" );
        if( gl )
        {
            gl->getGuis( &this->g_guis );
        }
        o.unlock();
        
        rl = (renderer_writelock *)o.writeLock( r, "renderer_gui_man::renderer_gui_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();
        
        this->_startTask( tp, 20, r );
    }
    
    //dtor
    renderer_gui_man::~renderer_gui_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_gui_man::~renderer_gui_man" );
        this->deleteGuis();
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "renderer_gui_man::~renderer_gui_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "renderer_gui_man::~renderer_gui_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        delete this->sdr;
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_guis;
        delete this->tpr;
        o.unlock();
    }
    
    //compute matrix
    void renderer_gui_man::computeMatrix( void )
    {
        float sw, sh, rw, rh, r, dw, dh, ss, w, h;
        shared_obj_guard o;
        render_api_context_readlock *l;
        
        l = (render_api_context_readlock *)o.tryReadLock( this->ctx, 100, "renderer_gui_man::computeMatrix" );
        if( !l )
            return;
        l->getDimensions( &w, &h );
        o.unlock();
        
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
        
        this->m.setOrtho( -dw, sh + dh, 0.0f, sw + dw, -dh, ss );
        this->m_undo.inverse( &this->m );
    }
    
    //return core
    core *renderer_gui_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *renderer_gui_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_gui_man_readlock( (renderer_gui_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_gui_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_gui_man_writelock( (renderer_gui_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_gui_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_gui_man_ref( (renderer_gui_man *)p, k );
    }
    
    //start task
    void renderer_gui_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay, renderer *r )
    {
        this->gtsk = new renderer_gui_man_task( this, r );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, "renderer_gui_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void renderer_gui_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "renderer_gui_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void renderer_gui_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //sync guis
    void renderer_gui_man::sync( dpthread_lock *thd, renderer_gui_man_ref *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pwl;
        renderer_gui_readlock *pl;
        shared_obj_guard o, o1, o2;
        renderer_gui_man_readlock *grl;
        renderer_gui_man_writelock *gwl;
        uint64_t t;
        gui_man_readlock *mrl;
        std::list<gui_ref *> lg, ng;
        std::list<gui_ref *>::iterator ig;
        gui_ref *pg;
        dpid_btree pt;
        gui_readlock *gl;
        gui_writelock *gw;
        
        grl = (renderer_gui_man_readlock *)o1.tryReadLock( g, 100, "renderer_gui_man::sync" );
        if( !grl )
            return;
        t = thd->getTicks();
        if( t - grl->t->t_last_gui_synced < 100 )
            return;
        grl->t->t_last_gui_synced = t;
        
        //build list of guis
        l = &grl->t->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 100, "renderer_gui_man::sync" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), p );
        }
        o.unlock();
        
        mrl = (gui_man_readlock *)o2.tryReadLock( grl->t->g_guis, 100, "renderer_gui_man::sync" );
        if( !mrl )
            return;
        
        //find guis, sync them, and remove from list
        mrl->getGuis( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            gl = (gui_readlock *)o.tryReadLock( pg, 1000, "renderer_gui_man::sync" );
            if( !gl )
                continue;
            p = (renderer_gui *)pt.findLeaf( gl->getId() );
            if( !p )
                ng.push_back( pg );
            else
                pt.removeLeaf( p );
        }
        o.unlock();
        
        //build list of guis to delete
        l = &grl->t->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 100, "renderer_gui_man::sync" );
            if( !pl )
                continue;
            if( pt.findLeaf( pl->getId() ) )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() && ng.empty() )
            return;
        
        gwl = (renderer_gui_man_writelock *)o1.tryWriteLock( g, 100, "renderer_gui_man::sync" );
        if( !gwl )
            return;
        
        //kill unmatched guis
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pwl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::sync" );
            if( !pwl )
                continue;
            pwl->kill();
        }
        o.unlock();
        
        //create new guis
        for( ig = ng.begin(); ig != ng.end(); ++ig )
        {
            pg = *ig;
            gw = (gui_writelock *)o.tryWriteLock( pg, 1000, "renderer_gui_man::sync" );
            if( !gw )
                continue;
            p = gwl->t->genGui( gw );
            if( p )
            {
                gw->setRenderer( p );
                gwl->t->guis.push_back( p );
            }
        }
        o.unlock();

    }
    
    //delete old guis
    void renderer_gui_man::deleteOldGuis( dpthread_lock *thd, renderer_gui_man_ref *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o, o1;
        renderer_gui_man_writelock *grl;
        
        grl = (renderer_gui_man_writelock *)o1.tryReadLock( g, 100, "renderer_gui_man::runGuis" );
        if( !grl )
            return;
        
        l = &grl->t->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::runGuis" );
            if( !pl )
                continue;
            if( !pl->isAlive() )
                d.push_back( p );
        }
        o.unlock();
        o1.unlock();
        
        if( d.empty() )
            return;
        grl = (renderer_gui_man_writelock *)o1.tryReadLock( g, 100, "renderer_gui_man::runGuis" );
        if( !grl )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            grl->t->guis.remove( p );
            delete p;
        }
    }
    
    //run guis
    void renderer_gui_man::runGuis( dpthread_lock *thd, renderer_gui_man_ref *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o, o1, oc;
        renderer_gui_man_writelock *grl;
        dpid pid;
        render_api_context_writelock *ctxl;
        
        grl = (renderer_gui_man_writelock *)o1.tryReadLock( g, 100, "renderer_gui_man::runFromTask" );
        if( !grl )
            return;
        ctxl = (render_api_context_writelock *)oc.tryWriteLock( grl->t->ctx, 100, "renderer_gui_man::runFromTask" );
        if( !ctxl )
            return;
        
        grl->t->computeMatrix();
        
        l = &grl->t->guis;
        grl->t->focus_gui = dpid_null();
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::runFromTask" );
            if( !pl )
                continue;
            pl->run( thd, ctxl );
            if( pl->isAlive() && ( pl->hasFocus() || dpid_isZero( &grl->t->focus_gui ) ) )
            {
                if( !pl->getFocusChild( grl, &grl->t->focus_gui ) )
                    grl->t->focus_gui = pl->getId();
            }
            pid = pl->getParentId();
            if( dpid_isZero( &pid ) )
                pl->redoMatrix( thd, grl, 0 );
        }
        o.unlock();
        oc.unlock();
        o1.unlock();
    }
    
    //run from manager thread
    void renderer_gui_man::run( dpthread_lock *thd, renderer_gui_man_ref *g, renderer_ref *r )
    {
        renderer_gui_man::sync( thd, g );
        renderer_gui_man::runGuis( thd, g );
        renderer_gui_man::deleteOldGuis( thd, g );
        renderer_gui_man::render( thd, g );
        if( !renderer_gui_man::waitForRenderer( r ) )
            return;
        renderer_gui_man::swapList( g, r );
    }
    
    //delete guis
    void renderer_gui_man::deleteGuis( void )
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
    
    //render guis into commandlist
    void renderer_gui_man::render( dpthread_lock *thd, renderer_gui_man_ref *g )
    {
        renderer_gui_man_writelock *wl;
        shared_obj_guard o, octxt, ocl;
        render_api_context_writelock *ctxl;
        render_api_commandlist_writelock *cll;
        
        wl = (renderer_gui_man_writelock *)o.tryWriteLock( g, 100, "renderer_gui_man::render" );
        if( !wl )
            return;
        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( wl->t->ctx, 100, "renderer_gui_man::render" );
        if( !ctxl )
            return;
        
        if( wl->t->clist )
            delete wl->t->clist;
        wl->t->clist = ctxl->makeCmdList();
        if( !wl->t->clist )
            return;
        
        cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( wl->t->clist, 100, "renderer_gui_man::render" );
        if( !cll )
            return;
        
        cll->setShader( wl->t->sdr );
        wl->renderGuis( thd, &wl->t->m, ctxl, cll );
        
        cll->compile( ctxl );
        cll->deleteCommands();
    }
    
    //wait for renderer to finish with commandlist
    bool renderer_gui_man::waitForRenderer( renderer_ref *r )
    {
        renderer_readlock *rl;
        shared_obj_guard o;
        int t;
        
        for( t = 0; t < 1000; t++ )
        {
            rl = (renderer_readlock *)o.tryReadLock( r, "renderer_gui_man::waitForRenderer" );
            if( !rl )
                return 0;
            if( !rl->isGuiCommandListUploaded() )
                return 1;
            o.unlock();
            std::this_thread::sleep_for( std::chrono::milliseconds( 0 ) );
        }
        
        return 0;
    }
    
    //swap command list with renderer
    void renderer_gui_man::swapList( renderer_gui_man_ref *g, renderer_ref *r )
    {
        renderer_writelock *rl;
        shared_obj_guard o, o1;
        renderer_gui_man_writelock *wl;

        rl = (renderer_writelock *)o.tryWriteLock( r, 1000, "renderer_gui_man::swapList" );
        if( !rl )
            return;
        wl = (renderer_gui_man_writelock *)o1.tryWriteLock( g, 1000, "renderer_gui_man::swapList" );
        if( !wl )
            return;
        
        rl->uploadGuiCommandList( wl->t->clist );
        wl->t->clist = 0;
    }
    
    //render guis
    void renderer_gui_man::renderGuis( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl )
    {
        std::list<renderer_gui *> *l, lz, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
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
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::renderGuis" );
                if( !pl )
                    continue;
                pl->render( thd, ml, m_world, ctx, cl );
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
    
    //return guis
    void renderer_gui_man::getChildrenGuis( std::list<renderer_gui *> *ll, dpid pid )
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
    bool renderer_gui_man::processGuiMouseInput( renderer_gui_man_writelock *r, float w, float h, float x, float y, bool lb, bool rb )
    {
        std::list<renderer_gui *> *l, lz, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        dpid nid;
        int max_z, z;
        dpxyz_f t;
        
        x = x / w;
        y = y / h;
        
        x = ( 2.0f * x ) - 1.0f;
        y = ( 2.0f * y ) - 1.0f;
        y = -y;
        
        t.x = x;
        t.y = y;
        t.z = 0;
        this->m_undo.transform( &t );

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
    dpid renderer_gui_man::getHoverId( void )
    {
        return this->hover_gui;
    }
    
    //process gui keyboard input
    void renderer_gui_man::processGuiKbInput( std::string *skey_name, bool isDown )
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
    bool renderer_gui_man::getSelectedText( std::string *s, bool bDoCut )
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
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::getSelectedText" );
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
    bool renderer_gui_man::setSelectedText( std::string *s )
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
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::setSelectedText" );
            if( !pl )
                return 0;
            if( !pl->compareId( this->focus_gui ) )
                continue;
            pl->setSelectedText( s );
            return 1;
        }
        
        return 0;
    }
    
    //generate renderer gui
    renderer_gui *renderer_gui_man::genGui( gui_writelock *ml )
    {
        return new renderer_gui( ml );
    }
    
};
