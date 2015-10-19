
#include "renderer_state_start_api.h"
#include "renderer_state_start_gui.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_start_api::renderer_state_start_api( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_start_api::~renderer_state_start_api( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_start_api::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->startApi( thd, rl );
        return new renderer_state_start_gui( r );
    }
    
};
