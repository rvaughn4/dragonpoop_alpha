
#include "renderer_state_start_gui.h"
#include "renderer_state_start_model.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_start_gui::renderer_state_start_gui( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_start_gui::~renderer_state_start_gui( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_start_gui::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->startGui( thd, rl );
        return new renderer_state_start_model( r );
    }
    
};
