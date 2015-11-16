
#include "renderer_state_init_gui.h"
#include "renderer_state_init_model.h"
#include "renderer_state_deinit_gui.h"

namespace dragonpoop
{

    //ctor
    renderer_state_init_gui::renderer_state_init_gui( renderer *r ) : renderer_state( r )
    {
        this->tries = 0;
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
        this->tries++;
        if( this->tries < 20 )
            return 0;
        return new renderer_state_deinit_gui( r );
    }

};
