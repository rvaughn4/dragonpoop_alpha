
#include "renderer_state.h"
#include "renderer.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state::renderer_state( renderer *r )
    {
        this->r = r;
    }
    
    //dtor
    renderer_state::~renderer_state( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        return 0;
    }
    
};
