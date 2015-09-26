
#include "gui_readlock.h"
#include "gui.h"

namespace dragonpoop
{
    
    //ctor
    gui_readlock::gui_readlock( gui *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_readlock::~gui_readlock( void )
    {
        
    }
    
    //return core
    core *gui_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //returns id
    dpid gui_readlock::getId( void )
    {
        return this->t->getId();
    }
    
    //compares id
    bool gui_readlock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }
    
    //get dimensions
    void gui_readlock::getDimensions( gui_dims *p )
    {
        this->t->getDimensions( p );
    }

    //returns true if has background texture
    bool gui_readlock::hasBg( void )
    {
        return this->t->hasBg();
    }
    
    //returns true if has forground texture
    bool gui_readlock::hasFg( void )
    {
        return this->t->hasFg();
    }
    
    //get parent id
    dpid gui_readlock::getParentId( void )
    {
        return this->t->getParentId();
    }
    
};
