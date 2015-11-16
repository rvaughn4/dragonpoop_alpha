
#include "renderer_state_init_api.h"
#include "renderer_state_init_gui.h"
#include "renderer_state_deinit_api.h"

namespace dragonpoop
{

    //ctor
    renderer_state_init_api::renderer_state_init_api( renderer *r ) : renderer_state( r )
    {
        this->tries = 0;
    }

    //dtor
    renderer_state_init_api::~renderer_state_init_api( void )
    {

    }

    //run, returns next state
    renderer_state *renderer_state_init_api::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        if( this->initApi( thd, rl ) )
            return new renderer_state_init_gui( r );
        this->tries++;
        if( this->tries < 20 )
            return 0;
        return new renderer_state_deinit_api( r );
    }

};
