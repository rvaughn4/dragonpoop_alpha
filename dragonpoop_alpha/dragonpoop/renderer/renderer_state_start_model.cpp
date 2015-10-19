
#include "renderer_state_start_model.h"
#include "renderer_state_run.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_start_model::renderer_state_start_model( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_start_model::~renderer_state_start_model( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_start_model::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->startModel( thd, rl );
        return new renderer_state_run( r );
    }
    
};
