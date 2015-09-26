
#include "gui.h"
#include "gui_ref.h"
#include "gui_readlock.h"
#include "gui_writelock.h"
#include "../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    gui::gui( core *c, dpid id ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->id = id;
        this->bHasBg = this->bHasFg = 0;
        this->bPosChanged = this->bTexChanged = 0;
        this->bRedraw = 1;
        this->bRepaintBg = this->bRepaintFg = 0;
    }
    
    //dtor
    gui::~gui( void )
    {
        
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
    void gui::run( dpthread_lock *thd, gui_writelock *g )
    {
        
        if( this->bRedraw )
        {
            if( this->bRepaintBg && this->bHasBg )
            {
                this->bgtex.resize( this->pos.w, this->pos.h );
                this->repaintBg( g, &this->bgtex, this->pos.w, this->pos.h );
                this->bTexChanged = 1;
            }
            
            if( this->bRepaintFg && this->bHasFg )
            {
                this->fgtex.resize( this->pos.w, this->pos.h );
                this->repaintFg( g, &this->fgtex, this->pos.w, this->pos.h );
                this->bTexChanged = 1;
            }
            
            this->bRedraw = 0;
        }
        
        if( this->bPosChanged || this->bTexChanged )
        {
            //sync with renderer
            this->bPosChanged = this->bTexChanged = 0;
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
    
};
