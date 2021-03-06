
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
#include "../../core/dpthread/dpthread_singletask.h"
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
#include "../renderer_commandlist_passer.h"
#include "../renderer_commandlist_passer_ref.h"
#include "../renderer_commandlist_passer_writelock.h"
#include "../api_stuff/input_passer/input_passer.h"
#include "../api_stuff/input_passer/input_passer_ref.h"
#include "../api_stuff/input_passer/input_passer_writelock.h"

#include <math.h>
#include <thread>
#include <iostream>

namespace dragonpoop
{

    //ctor
    renderer_gui_man::renderer_gui_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height, input_passer_writelock *ipl ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        renderer_writelock *rl;
        renderer_commandlist_passer_writelock *cpl;

        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( clpasser, 5000, "renderer_gui_man::renderer_gui_man" );
        if( cpl )
            this->clpasser = (renderer_commandlist_passer_ref *)cpl->getRef();
        else
            this->clpasser = 0;
        o.unlock();

        this->listReady = 1;
        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        this->focus_gui = dpid_null();
        this->hover_gui = this->focus_gui;
        this->ip = new input_passer( c->getMutexMaster() );
        this->ipr = (input_passer_ref *)ipl->getRef();
        this->lb = this->rb = 0;

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

        this->_startTask( tp, 5 );
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
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_guis;
        delete this->tpr;
        delete this->clpasser;
        delete this->ipr;
        delete this->ip;
        o.unlock();
    }

    //compute matrix
    void renderer_gui_man::computeMatrix( void )
    {
        this->m.setIdentity();
        this->m_undo.setIdentity();
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
    void renderer_gui_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new renderer_gui_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, 1, "renderer_gui_man" );
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

    //run from manager thread
    void renderer_gui_man::run( dpthread_lock *thd, renderer_gui_man_writelock *g )
    {
        shared_obj_guard o;
        input_passer_writelock *ipl;
        float x, y, w, h;
        bool bLeft, bRight, bDown;
        std::string skey;

        w = 1920;
        h = 1080;

        this->sync( thd, g );
        this->runGuis( thd, g );
        this->deleteOldGuis( thd, g );
        this->render( thd, g );

        ipl = (input_passer_writelock *)o.tryWriteLock( this->ip, 30, "renderer_gui_man::run" );
        if( !ipl )
            return;
        ipl->sync( this->ipr );

        if( !ipl->hasInput() )
            return;

        if( ipl->getMouseInput( &x, &y, &bLeft, &bRight, &bDown ) )
        {
            if( bDown )
            {
                this->lb |= bLeft;
                this->rb |= bRight;
            }
            else
            {
                this->lb &= !bLeft;
                this->rb &= !bRight;
            }
            this->processGuiMouseInput( g, w, h, x, y, this->lb, this->rb );
        }

        if( ipl->getKeyboardInput( &skey, &bDown ) )
            this->processGuiKbInput( g, &skey, bDown );

    }

    //sync guis
    void renderer_gui_man::sync( dpthread_lock *thd, renderer_gui_man_writelock *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pwl;
        renderer_gui_readlock *pl;
        shared_obj_guard o, o1, o2;
        uint64_t t;
        gui_man_readlock *mrl;
        std::list<gui_ref *> lg, ng;
        std::list<gui_ref *>::iterator ig;
        gui_ref *pg;
        dpid_btree pt;
        gui_readlock *gl;
        gui_writelock *gw;

        t = thd->getTicks();
        if( t - this->t_last_gui_synced < 100 )
            return;
        this->t_last_gui_synced = t;

        //build list of guis
        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 100, "renderer_gui_man::sync" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), p );
        }
        o.unlock();

        mrl = (gui_man_readlock *)o2.tryReadLock( this->g_guis, 100, "renderer_gui_man::sync" );
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
        l = &this->guis;
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
            p = this->genGui( gw );
            if( p )
            {
                gw->setRenderer( p );
                this->guis.push_back( p );
            }
        }
        o.unlock();

    }

    //delete old guis
    void renderer_gui_man::deleteOldGuis( dpthread_lock *thd, renderer_gui_man_writelock *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o, o1;

        l = &this->guis;
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

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->guis.remove( p );
            delete p;
        }
    }

    //run guis
    void renderer_gui_man::runGuis( dpthread_lock *thd, renderer_gui_man_writelock *g )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o, oc;
        render_api_context_writelock *ctxl;

        ctxl = (render_api_context_writelock *)oc.tryWriteLock( this->ctx, 100, "renderer_gui_man::runFromTask" );
        if( !ctxl )
            return;

        ctxl->makeActive( thd );
        this->computeMatrix();

        l = &this->guis;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::runFromTask" );
            if( !pl )
                continue;
            pl->run( thd, ctxl, g );
            pl->redoMatrix( thd, g, 0 );
        }
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
    void renderer_gui_man::render( dpthread_lock *thd, renderer_gui_man_writelock *g )
    {
        shared_obj_guard o, octxt, ocl, ocpl;
        render_api_context_writelock *ctxl;
        render_api_commandlist_writelock *cll;
        renderer_commandlist_passer_writelock *cpl;
        render_api_shader_ref *sdr;

        if( renderer_commandlist_passer::waitForFlag( &this->clpasser->t->gui_ready, 0, 5 ) )
            return;

        cpl = (renderer_commandlist_passer_writelock *)ocpl.tryWriteLock( this->clpasser, 30, "renderer_gui_man::render" );
        if( !cpl )
            return;
        ctxl = (render_api_context_writelock *)octxt.tryWriteLock( this->ctx, 30, "renderer_gui_man::render" );
        if( !ctxl )
            return;
        cpl->setGui( g->t->clist );
        ocpl.unlock();

        ctxl->makeActive( thd );
        ctxl->run();
        if( this->clist )
            delete this->clist;
        this->clist = ctxl->makeCmdList();
        if( !this->clist )
            return;

        cll = (render_api_commandlist_writelock *)ocl.tryWriteLock( this->clist, 30, "renderer_gui_man::render" );
        if( !cll )
            return;

        sdr = ctxl->makeShader( "gui" );
        if( !sdr )
            return;

        cll->setShader( sdr );
        this->renderGuis( thd, g, &this->m, ctxl, cll );

        if( cll->compile( ctxl ) )
            this->clpasser->t->gui_ready = 1;

        delete sdr;
    }

    //render guis
    void renderer_gui_man::renderGuis( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        int max_z, z;

        l = &this->guis;
        max_z = 0;
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
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( z != (int)p->getZ() )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui_man::renderGuis" );
                if( !pl )
                    continue;
                pl->render( thd, ml, m_world, ctx, cl );
            }
        }
    }

    //process mouse input
    bool renderer_gui_man::processGuiMouseInput( renderer_gui_man_writelock *r, float w, float h, float x, float y, bool lb, bool rb )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        int max_z, z;
        dpxyz_f t;

        t.x = x;
        t.y = y;
        t.z = 0;
        this->m_undo.transform( &t );

        l = &this->guis;
        max_z = 0;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            z = p->getZ();
            if( z > max_z )
                max_z = z;
        }
        max_z++;

        this->hover_gui = dpid_null();
        this->focus_gui = dpid_null();
        for( z = 0; z < max_z; z++ )
        {
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( z != (int)p->getZ() )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer::processGuiMouseInput" );
                if( !pl )
                    continue;
                if( pl->processMouse( r, t.x, t.y, t.x, t.y, lb, rb, this->focus_gui ) )
                {
                    this->hover_gui = pl->getHoverId();
                    this->focus_gui = pl->getFocusId();
                    return 1;
                }
            }
        }

        return 0;
    }

    //get hovering gui id
    dpid renderer_gui_man::getHoverId( void )
    {
        return this->hover_gui;
    }

    //process gui keyboard input
    void renderer_gui_man::processGuiKbInput( renderer_gui_man_writelock *r, std::string *skey_name, bool isDown )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        unsigned int max_z, z;

        l = &this->guis;
        max_z = 0;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( p->getZ() >= max_z )
                max_z = p->getZ() + 1;
        }

        for( z = 0; z < max_z; z++ )
        {
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( p->getZ() != z )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 500, "renderer::processGuiKbInput" );
                if( !pl )
                    continue;
                if( pl->processKb( r, skey_name, isDown ) )
                    return;
            }
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
