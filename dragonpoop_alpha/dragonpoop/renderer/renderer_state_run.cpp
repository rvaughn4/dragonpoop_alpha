
#include "renderer_state_run.h"
#include "renderer_state_stop_model.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_run::renderer_state_run( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_run::~renderer_state_run( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_run::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->_run( thd, rl );
        if( this->doRun() )
            return 0;
        return new renderer_state_stop_model( r );
    }
    
};
