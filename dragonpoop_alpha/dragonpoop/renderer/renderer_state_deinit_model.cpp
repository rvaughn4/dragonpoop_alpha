
#include "renderer_state_deinit_model.h"
#include "renderer_state_deinit_gui.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_deinit_model::renderer_state_deinit_model( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_deinit_model::~renderer_state_deinit_model( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_deinit_model::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->deinitModel( thd, rl );
        return new renderer_state_deinit_gui( r );
    }
    
};
