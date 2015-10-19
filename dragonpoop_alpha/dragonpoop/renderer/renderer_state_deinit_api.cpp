
#include "renderer_state_deinit_api.h"
#include "renderer_state_close.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_deinit_api::renderer_state_deinit_api( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_deinit_api::~renderer_state_deinit_api( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_deinit_api::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->deinitApi( thd, rl );
        return new renderer_state_close( r );
    }
    
};
