
#include "window.h"

namespace dragonpoop
{
    
    //ctor
    window::window( float w, float h, const char *ctitle )
    {
        
    }
    
    //dtor
    window::~window( void )
    {
        
    }
    
    //run window
    void window::run( void )
    {
        
    }
    
    //returns true if window is open
    bool window::isOpen( void )
    {
        return 0;
    }
    
    //returns window width
    float window::getWidth( void )
    {
        return 0;
    }
    
    //returns window height
    float window::getHeight( void )
    {
        return 0;
    }

    //returns true if mouse input is waiting
    bool window::hasMouseInput( void )
    {
        return !this->mse.empty();
    }
    
    //fetches mouse input
    bool window::getMouseInput( window_mouse_input *m )
    {
        if( this->mse.empty() )
            return 0;
        
        *m = this->mse.front();
        this->mse.pop();
        
        return 1;
    }
    
    //adds mouse input
    void window::addMouseInput( window_mouse_input *m )
    {
        this->mse.push( *m );
    }
    
    //returns true if kb input is waiting
    bool window::hasKBInput( void )
    {
        return !this->kbe.empty();
    }
    
    //fetches kb input
    bool window::getKBInput( window_kb_input *m )
    {
        if( this->kbe.empty() )
            return 0;
        
        *m = this->kbe.front();
        this->kbe.pop();
        
        return 1;
    }
    
    //adds kb input
    void window::addKBInput( window_kb_input *m )
    {
        this->kbe.push( *m );
    }
    
};
