
#include "renderer_state_deinit_gui.h"
#include "renderer_state_deinit_api.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_deinit_gui::renderer_state_deinit_gui( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_deinit_gui::~renderer_state_deinit_gui( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_deinit_gui::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        this->deinitGui( thd, rl );
        return new renderer_state_deinit_api( r );
    }
    
};
