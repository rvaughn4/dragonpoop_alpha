
#include "renderer_state_close.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_close::renderer_state_close( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_close::~renderer_state_close( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_close::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->kill();
        return 0;
    }

};
