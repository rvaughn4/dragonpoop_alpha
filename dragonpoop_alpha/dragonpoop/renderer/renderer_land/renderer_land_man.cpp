
#include "renderer_land_man.h"
#include "renderer_land_man_ref.h"
#include "renderer_land_man_readlock.h"
#include "renderer_land_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "renderer_land_man_task.h"
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
#include "../../gfx/dpland/dpland_man_ref.h"
#include "../../gfx/dpland/dpland_man_readlock.h"
//#include "renderer_land.h"
#include "../../gfx/dpland/dpland.h"
#include "../../core/dpbtree/dpid_btree.h"
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
    renderer_land_man::renderer_land_man( core *c, renderer *r, dptaskpool_writelock *tp, render_api_context_ref *ctx, renderer_commandlist_passer *clpasser, float log_screen_width, float log_screen_height ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        renderer_writelock *rl;
        renderer_commandlist_passer_writelock *cpl;
        
        cpl = (renderer_commandlist_passer_writelock *)o.tryWriteLock( clpasser, 5000, "renderer_land_man::renderer_land_man" );
        if( cpl )
            this->clpasser = (renderer_commandlist_passer_ref *)cpl->getRef();
        else
            this->clpasser = 0;
        o.unlock();
        
        this->log_screen_height = log_screen_height;
        this->log_screen_width = log_screen_width;
        this->ctx = ctx;
        this->clist = 0;
        this->c = c;
        this->tpr = (dptaskpool_ref *)tp->getRef();
        
        this->g = c->getGfx();
        gl = (gfx_writelock *)o.writeLock( this->g, "renderer_land_man::renderer_land_man" );
        if( gl )
        {
//            gl->getlands( &this->g_lands );
        }
        o.unlock();
        
        rl = (renderer_writelock *)o.writeLock( r, "renderer_land_man::renderer_land_man" );
        if( rl )
            this->r = (renderer_ref *)rl->getRef();
        o.unlock();
        
        this->thd = new dpthread_singletask( c->getMutexMaster(), 302 );
        this->_startTask( tp, 200 );
    }
    
    //dtor
    renderer_land_man::~renderer_land_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->deleteLands();
        o.unlock();
        this->unlink();
        delete this->thd;
        
        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "renderer_land_man::~renderer_land_man" );
        this->_deleteTask();
        if( this->clist )
            delete this->clist;
        delete this->ctx;
        delete this->r;
        delete this->g;
        delete this->g_lands;
        delete this->tpr;
        delete this->clpasser;
        o.unlock();
    }
    
    //compute matrix
    void renderer_land_man::computeMatrix( void )
    {
        float sw, sh, rw, rh, r, dw, dh, w, h;
        
        w = this->clpasser->t->w;
        h = this->clpasser->t->h;
        sw = log_screen_width;
        sh = log_screen_height;
        
        rw = sw / w;
        rh = sh / h;
        
        r = rw;
        if( r < rh )
            r = rh;
        dw = r - rw;
        dh = r - rh;
        
        this->m.setPerspective( -r - dw, -r - dh, 1.0f, r + dw, r + dh, 100.0f, 45.0f );
    }
    
    //return core
    core *renderer_land_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *renderer_land_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_land_man_readlock( (renderer_land_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_land_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_land_man_writelock( (renderer_land_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_land_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_land_man_ref( (renderer_land_man *)p, k );
    }
    
    //start task
    void renderer_land_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        dpthread_lock *thdl;
        
        this->gtsk = new renderer_land_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 1, "renderer_land_man" );
        
        thdl = this->thd->lock();
        if( thdl )
        {
            thdl->addTask( this->tsk );
            delete thdl;
        }
    }
    
    //kill task
    void renderer_land_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "renderer_land_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void renderer_land_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //sync lands
    void renderer_land_man::sync( dpthread_lock *thd )
    {
/*        std::list<renderer_land *> *l, d;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;
        renderer_land_writelock *pwl;
        renderer_land_readlock *pl;
        shared_obj_guard o, o1, o2;
        renderer_land_man_readlock *grl;
        renderer_land_man_writelock *gwl;
        uint64_t t;
        land_man_readlock *mrl;
        std::list<land_ref *> lg, ng;
        std::list<land_ref *>::iterator ig;
        land_ref *pg;
        dpid_btree pt;
        land_readlock *gl;
        land_writelock *gw;
        
        grl = (renderer_land_man_readlock *)o1.tryReadLock( g, 100, "renderer_land_man::sync" );
        if( !grl )
            return;
        t = thd->getTicks();
        if( t - grl->t->t_last_land_synced < 100 )
            return;
        grl->t->t_last_land_synced = t;
        
        //build list of lands
        l = &grl->t->lands;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_land_readlock *)o.tryReadLock( p, 100, "renderer_land_man::sync" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), p );
        }
        o.unlock();
        
        mrl = (land_man_readlock *)o2.tryReadLock( grl->t->g_lands, 100, "renderer_land_man::sync" );
        if( !mrl )
            return;
        
        //find lands, sync them, and remove from list
        mrl->getlands( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            gl = (land_readlock *)o.tryReadLock( pg, 1000, "renderer_land_man::sync" );
            if( !gl )
                continue;
            p = (renderer_land *)pt.findLeaf( gl->getId() );
            if( !p )
                ng.push_back( pg );
            else
                pt.removeLeaf( p );
        }
        o.unlock();
        
        //build list of lands to delete
        l = &grl->t->lands;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_land_readlock *)o.tryReadLock( p, 100, "renderer_land_man::sync" );
            if( !pl )
                continue;
            if( pt.findLeaf( pl->getId() ) )
                d.push_back( p );
        }
        o.unlock();
        
        if( d.empty() && ng.empty() )
            return;
        
        gwl = (renderer_land_man_writelock *)o1.tryWriteLock( g, 100, "renderer_land_man::sync" );
        if( !gwl )
            return;
        
        //kill unmatched lands
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pwl = (renderer_land_writelock *)o.tryWriteLock( p, 100, "renderer_land_man::sync" );
            if( !pwl )
                continue;
            pwl->kill();
        }
        o.unlock();
        
        //create new lands
        for( ig = ng.begin(); ig != ng.end(); ++ig )
        {
            pg = *ig;
            gw = (land_writelock *)o.tryWriteLock( pg, 1000, "renderer_land_man::sync" );
            if( !gw )
                continue;
            p = gwl->t->genland( gw );
            if( p )
            {
                gw->setRenderer( p );
                gwl->t->lands.push_back( p );
            }
        }
        o.unlock();
        */
    }
    
    //run lands
    void renderer_land_man::runLands( dpthread_lock *thd )
    {
/*        std::list<renderer_land *> *l, d;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;
        renderer_land_writelock *pl;
        shared_obj_guard o, o1, oc;
        renderer_land_man_writelock *grl;
        dpid pid;
        render_api_context_writelock *ctxl;
        
        grl = (renderer_land_man_writelock *)o1.tryReadLock( g, 100, "renderer_land_man::runFromTask" );
        if( !grl )
            return;
        ctxl = (render_api_context_writelock *)oc.tryWriteLock( grl->t->ctx, 100, "renderer_land_man::runFromTask" );
        if( !ctxl )
            return;
        
        grl->t->computeMatrix();
        
        l = &grl->t->lands;
        grl->t->focus_land = dpid_null();
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_land_writelock *)o.tryWriteLock( p, 100, "renderer_land_man::runFromTask" );
            if( !pl )
                continue;
            pl->run( thd, ctxl );
            if( pl->isAlive() && ( pl->hasFocus() || dpid_isZero( &grl->t->focus_land ) ) )
            {
                if( !pl->getFocusChild( grl, &grl->t->focus_land ) )
                    grl->t->focus_land = pl->getId();
            }
            pid = pl->getParentId();
            if( dpid_isZero( &pid ) )
                pl->redoMatrix( thd, grl, 0 );
        }
        o.unlock();
        oc.unlock();
        o1.unlock();
 */
    }
    
    //run from manager thread
    void renderer_land_man::run( dpthread_lock *thd, renderer_land_man_writelock *l )
    {

    }
    
    //delete lands
    void renderer_land_man::deleteLands( void )
    {
        std::list<renderer_land *> *l, d;
        std::list<renderer_land *>::iterator i;
        renderer_land *p;
        
        l = &this->lands;
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
         //   delete p;
        }
    }
    
    //render lands
    void renderer_land_man::renderLands( dpthread_lock *thd, renderer_land_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl )
    {

    }
    
    //generate renderer land
    renderer_land *renderer_land_man::genLand( dpland *ml )
    {
//        return new renderer_land( ml );
        return 0;
    }
    
};
