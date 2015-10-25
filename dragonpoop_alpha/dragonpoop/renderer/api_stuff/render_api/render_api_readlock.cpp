
#include "render_api_readlock.h"
#include "render_api.h"

namespace dragonpoop
{
    
    //ctor
    render_api_readlock::render_api_readlock( render_api *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_readlock::~render_api_readlock( void )
    {
        
    }
    
    //returns true if window is open
    bool render_api_readlock::isOpen( void )
    {
        return this->t->isOpen();
    }
    
    //returns window width
    float render_api_readlock::getWidth( void )
    {
        return this->t->getWidth();
    }
    
    //returns window height
    float render_api_readlock::getHeight( void )
    {
        return this->t->getHeight();
    }
    
    //returns true if mouse input is waiting
    bool render_api_readlock::hasMouseInput( void )
    {
        return this->t->hasMouseInput();
    }
    
};
