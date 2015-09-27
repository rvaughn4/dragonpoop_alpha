
#include "gui_writelock.h"
#include "gui.h"

namespace dragonpoop
{

    //ctor
    gui_writelock::gui_writelock( gui *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_writelock::~gui_writelock( void )
    {
        
    }
    
    //run gui
    void gui_writelock::run( dpthread_lock *thd, gfx_writelock *gl )
    {
        this->t->run( thd, this, gl );
    }
    
    //return core
    core *gui_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //returns id
    dpid gui_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //compares id
    bool gui_writelock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }
    
    //set width and height
    void gui_writelock::setWidthHeight( float w, float h )
    {
        this->t->setWidthHeight( w, h );
    }

    //set top left pos
    void gui_writelock::setPosition( float x, float y )
    {
        this->setPosition( x, y );
    }
    
    //get dimensions
    void gui_writelock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }
    
    //cause redraw of background and forground texture
    void gui_writelock::redraw( void )
    {
        this->t->redraw();
    }
    
    //set background redraw mode
    void gui_writelock::enableBgRedraw( bool b )
    {
        this->t->enableBgRedraw( b );
    }
    
    //set forground redraw mode
    void gui_writelock::enableFgRedraw( bool b )
    {
        this->t->enableFgRedraw( b );
    }
    
    //set background mode
    void gui_writelock::enableBg( bool b )
    {
        this->t->enableBg( b );
    }
    
    //set forground mode
    void gui_writelock::enableFg( bool b )
    {
        this->t->enableFg( b );
    }
    
    //returns true if has background texture
    bool gui_writelock::hasBg( void )
    {
        return this->t->hasBg();
    }
    
    //returns true if has forground texture
    bool gui_writelock::hasFg( void )
    {
        return this->t->hasFg();
    }
    
    //set parent id
    void gui_writelock::setParentId( dpid id )
    {
        this->t->setParentId( id );
    }
    
    //get parent id
    dpid gui_writelock::getParentId( void )
    {
        return this->t->getParentId();
    }
    
    //returns pointer to bg texture
    dpbitmap *gui_writelock::getBg( void )
    {
        return this->t->getBg();
    }
    
    //returns pointer to fg texture
    dpbitmap *gui_writelock::getFg( void )
    {
        return this->t->getFg();
    }

    //returns z order
    unsigned int gui_writelock::getZ( void )
    {
        return this->t->getZ();
    }
    
    //sets focus
    void gui_writelock::setFocus( gfx_readlock *g )
    {
        this->t->setFocus( g );
    }
    
    //sets focus
    void gui_writelock::setFocus( gfx_writelock *g )
    {
        this->t->setFocus( g );
    }
    
    //returns true if has focus
    bool gui_writelock::hasFocus( void )
    {
        return this->t->hasFocus();
    }
    
    //process mouse input
    void gui_writelock::processMouse( float x, float y, bool lb, bool rb )
    {
        this->t->processMouse( x, y, lb, rb );
    }
    
    //returns true if has renderer
    bool gui_writelock::hasRenderer( void )
    {
        return this->t->hasRenderer();
    }
    
    //set renderer
    void gui_writelock::setRenderer( renderer_gui *g )
    {
        this->t->setRenderer( g );
    }

};
