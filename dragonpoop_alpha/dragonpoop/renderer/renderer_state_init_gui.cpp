
#include "renderer_state_init_gui.h"
#include "renderer_state_init_model.h"
#include "renderer_state_deinit_gui.h"

namespace dragonpoop
{
    
    //ctor
    renderer_state_init_gui::renderer_state_init_gui( renderer *r ) : renderer_state( r )
    {
        
    }
    
    //dtor
    renderer_state_init_gui::~renderer_state_init_gui( void )
    {
        
    }
    
    //run, returns next state
    renderer_state *renderer_state_init_gui::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        if( this->initGui( thd, rl ) )
            return new renderer_state_init_model( r );
        return new renderer_state_deinit_gui( r );
    }
    
};
