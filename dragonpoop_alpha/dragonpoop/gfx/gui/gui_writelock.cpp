
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
    
    //return core
    core *gui_writelock::getCore( void )
    {
        return this->t->getCore();
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
    
};
