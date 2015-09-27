
#include "gui.h"
#include "gui_ref.h"
#include "gui_readlock.h"
#include "gui_writelock.h"
#include "../../core/core.h"
#include "../gfx_writelock.h"
#include "../gfx_readlock.h"
#include "../gfx_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../renderer/renderer_gui/renderer_gui.h"
#include "../../renderer/renderer_gui/renderer_gui_ref.h"
#include "../../renderer/renderer_gui/renderer_gui_writelock.h"

namespace dragonpoop
{
    
    //ctor
    gui::gui( gfx_writelock *g, dpid id, dpid parent_id ) : shared_obj( g->getCore()->getMutexMaster() )
    {
        this->c = g->getCore();
        this->g = (gfx_ref *)g->getRef();
        this->id = id;
        this->pid = parent_id;
        this->bHasBg = this->bHasFg = 0;
        this->bPosChanged = this->bBgChanged = this->bFgChanged = 0;
        this->bRedraw = 1;
        this->bRepaintBg = this->bRepaintFg = 0;
        this->r = 0;
        this->bWasBgDrawn = this->bWasFgDrawn = 0;
        this->z = 1;
        this->bMouseInput = 0;
        this->bOldLb = this->bOldRb = 0;
        this->bLb = this->bRb = 0;
    }
    
    //dtor
    gui::~gui( void )
    {
        delete this->g;
        if( this->r )
            delete this->r;
    }
    
    //return core
    core *gui::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gui_readlock( (gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gui_writelock( (gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gui_ref( (gui *)p, k );
    }
    
    //run gui
    void gui::run( dpthread_lock *thd, gui_writelock *g, gfx_writelock *gl )
    {
        shared_obj_guard o;
        renderer_gui_writelock *l;
        
        if( this->bRedraw )
        {
            if( ( !this->bWasBgDrawn || this->bRepaintBg ) && this->bHasBg )
            {
                this->repaintBg( g, &this->bgtex, this->pos.w, this->pos.h );
                this->bBgChanged = 1;
                this->bWasBgDrawn = 1;
            }
            
            if( ( !this->bWasFgDrawn || this->bRepaintFg ) && this->bHasFg )
            {
                this->repaintFg( g, &this->fgtex, this->pos.w, this->pos.h );
                this->bFgChanged = 1;
                this->bWasFgDrawn = 1;
            }
            
            this->bRedraw = 0;
        }
        
        if( this->bPosChanged || this->bBgChanged || this->bFgChanged )
        {
            if( this->r )
            {
                l = (renderer_gui_writelock *)o.tryWriteLock( this->r, 500, "gui::run" );
                if( l )
                {
                    if( this->bPosChanged )
                        l->syncPos();
                    if( this->bBgChanged )
                        l->syncBg();
                    if( this->bFgChanged )
                        l->syncFg();
                }

                this->bPosChanged = this->bBgChanged = this->bFgChanged = 0;
            }
        }
        
        if( this->bMouseInput )
        {
            if( this->bOldLb != this->bLb )
                this->setFocus( gl );
            if( this->bOldLb != this->bLb )
                this->handleMouseClick( this->mx, this->my, 0, this->bLb );
            if( this->bOldRb != this->bRb )
                this->handleMouseClick( this->mx, this->my, 1, this->bRb );
            this->bMouseInput = 0;
            this->bOldLb = this->bLb;
            this->bOldRb = this->bRb;
        }
    }
    
    //returns id
    dpid gui::getId( void )
    {
        return this->id;
    }
    
    //compares id
    bool gui::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }
    
    //set width and height
    void gui::setWidthHeight( float w, float h )
    {
        this->pos.w = w;
        this->pos.h = h;
        this->redraw();
        this->bPosChanged = 1;
    }
    
    //set top left pos
    void gui::setPosition( float x, float y )
    {
        this->pos.x = x;
        this->pos.y = y;
        this->bPosChanged = 1;
    }
    
    //get dimensions
    void gui::getDimensions( gui_dims *p )
    {
        *p = this->pos;
    }
    
    //cause redraw of background and forground texture
    void gui::redraw( void )
    {
        this->bRedraw = 1;
    }
    
    //set background redraw mode
    void gui::enableBgRedraw( bool b )
    {
        this->bRepaintBg = b;
    }
    
    //set forground redraw mode
    void gui::enableFgRedraw( bool b )
    {
        this->bRepaintFg = b;
    }
    
    //set background mode
    void gui::enableBg( bool b )
    {
        this->bHasBg = b;
    }
    
    //set forground mode
    void gui::enableFg( bool b )
    {
        this->bHasFg = b;
    }
    
    //returns true if has background texture
    bool gui::hasBg( void )
    {
        return this->bHasBg;
    }
    
    //returns true if has forground texture
    bool gui::hasFg( void )
    {
        return this->bHasFg;
    }
    
    //override to paint background texture
    void gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        
    }
    
    //override to paint forground texture
    void gui::repaintFg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        
    }
    
    //set parent id
    void gui::setParentId( dpid id )
    {
        this->pid = id;
    }
    
    //get parent id
    dpid gui::getParentId( void )
    {
        return this->pid;
    }
    
    //returns true if has renderer
    bool gui::hasRenderer( void )
    {
        if( !this->r || !this->r->isLinked() )
            return 0;
        return 1;
    }
    
    //set renderer
    void gui::setRenderer( renderer_gui *g )
    {
        shared_obj_guard o;
        renderer_gui_writelock *l;
        
        if( this->r )
            delete this->r;
        this->r = 0;
        
        l = (renderer_gui_writelock *)o.tryWriteLock( g, 300, "gui::setRenderer" );
        if( !l )
            return;
        this->r = (renderer_gui_ref *)l->getRef();
    }

    //returns pointer to bg texture
    dpbitmap *gui::getBg( void )
    {
        return &this->bgtex;
    }
    
    //returns pointer to fg texture
    dpbitmap *gui::getFg( void )
    {
        return &this->fgtex;
    }
    
    //returns z order
    unsigned int gui::getZ( void )
    {
        return this->z;
    }
    
    //sets focus
    void gui::setFocus( gfx_readlock *g )
    {
        std::list<gui_ref *> l;
        g->getGuis( &l );
        this->setFocus( &l );
    }
    
    //sets focus
    void gui::setFocus( gfx_writelock *g )
    {
        std::list<gui_ref *> l;
        g->getGuis( &l );
        this->setFocus( &l );
    }
    
    //sets focus
    void gui::setFocus( std::list<gui_ref *> *l )
    {
        shared_obj_guard o;
        gui_writelock *pl;
        gui_ref *p;
        std::list<gui_ref *>::iterator i;
        
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (gui_writelock *)o.tryWriteLock( p, 100, "gui::setFocus" );
            if( !pl )
                continue;
            if( pl->t->z < 20 )
            {
                pl->t->z++;
                pl->t->bPosChanged = 1;
            }
        }
        o.unlock();
        
        this->bPosChanged = 1;
        this->z = 0;
    }
    
    //returns true if has focus
    bool gui::hasFocus( void )
    {
        return this->z == 0;
    }
 
    //process mouse input
    void gui::processMouse( float x, float y, bool lb, bool rb )
    {
        this->mx = x;
        this->my = y;
        this->bLb = lb;
        this->bRb = rb;
        this->bMouseInput = 1;
    }
    
    //override to handle mouse button
    void gui::handleMouseClick( float x, float y, bool isRight, bool isDown )
    {
        
    }
    
};
