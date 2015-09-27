
#include "renderer_gui.h"
#include "renderer_gui_readlock.h"
#include "renderer_gui_writelock.h"
#include "renderer_gui_ref.h"
#include "../../gfx/gui/gui_writelock.h"
#include "../../gfx/gui/gui_ref.h"
#include "../../core/core.h"
#include "../renderer_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    renderer_gui::renderer_gui( gui_writelock *g ) : shared_obj( g->getCore()->getMutexMaster() )
    {
        this->g = (gui_ref *)g->getRef();
        this->c = g->getCore();
        this->id = g->getId();
        this->pid = g->getParentId();
        g->getDimensions( &this->pos );
        this->bHasBg = g->hasBg();
        this->bHasFg = g->hasFg();
    }
    
    //dtor
    renderer_gui::~renderer_gui( void )
    {
        delete g;
    }
    
    //return core
    core *renderer_gui::getCore( void )
    {
        return this->c;
    }
    
    //compares parent id
    bool renderer_gui::compareParentId( dpid id )
    {
        return dpid_compare( &id, &this->pid );
    }
    
    //generate read lock
    shared_obj_readlock *renderer_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_gui_readlock( (renderer_gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_gui_writelock( (renderer_gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_gui_ref( (renderer_gui *)p, k );
    }
    
    //run gui
    void renderer_gui::run( dpthread_lock *thd, renderer_gui_writelock *g )
    {
        //gui_writelock
    }
    
    //render model
    void renderer_gui::render( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world )
    {
        std::list<renderer_gui *> l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_readlock *pl;
        shared_obj_guard o;
        
        r->getChildrenGuis( &l, this->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 30, "renderer_gui::render" );
            if( !pl )
                continue;
            pl->render( thd, r, m_world );
        }
        
        r->renderGui( thd, m, m_world );
    }
    
    //returns id
    dpid renderer_gui::getId( void )
    {
        return this->id;
    }
    
    //compares id
    bool renderer_gui::compareId( dpid id )
    {
        return dpid_compare( &id, &this->id );
    }
    
    //get dimensions
    void renderer_gui::getDimensions( gui_dims *p )
    {
        *p = this->pos;
    }
    
    //returns true if has background texture
    bool renderer_gui::hasBg( void )
    {
        return this->bHasBg;
    }
    
    //returns true if has forground texture
    bool renderer_gui::hasFg( void )
    {
        return this->bHasFg;
    }
    
    //get parent id
    dpid renderer_gui::getParentId( void )
    {
        return this->pid;
    }
    
    //override to handle bg texture update
    void renderer_gui::updateBg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
    //override to handle fg texture update
    void renderer_gui::updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
    //called to force pos update
    void renderer_gui::syncPos( void )
    {
        this->bSyncPos = 1;
    }
    
    //called to force bg update
    void renderer_gui::syncBg( void )
    {
        this->bSyncBg = 1;
    }
    
    //called to force fg update
    void renderer_gui::syncFg( void )
    {
        this->bSyncFg = 1;
    }
    
};
