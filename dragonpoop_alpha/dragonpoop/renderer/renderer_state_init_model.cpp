
#include "renderer_state_init_model.h"
#include "renderer_state_start_api.h"
#include "renderer_state_deinit_model.h"

namespace dragonpoop
{

    //ctor
    renderer_state_init_model::renderer_state_init_model( renderer *r ) : renderer_state( r )
    {
        this->tries = 0;
    }

    //dtor
    renderer_state_init_model::~renderer_state_init_model( void )
    {

    }

    //run, returns next state
    renderer_state *renderer_state_init_model::run( renderer *r, renderer_writelock *rl, dpthread_lock *thd )
    {
        if( this->initModel( thd, rl ) )
            return new renderer_state_start_api( r );
        this->tries++;
        if( this->tries < 20 )
            return 0;
        return new renderer_state_deinit_model( r );
    }

};
