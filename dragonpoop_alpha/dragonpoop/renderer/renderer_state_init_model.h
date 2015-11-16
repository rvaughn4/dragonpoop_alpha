
#ifndef dragonpoop_renderer_state_init_model_h
#define dragonpoop_renderer_state_init_model_h

#include "renderer_state.h"

namespace dragonpoop
{

    class renderer_state_init_model : public renderer_state
    {

    private:

        int tries;

    protected:


    public:

        //ctor
        renderer_state_init_model( renderer *r );
        //dtor
        virtual ~renderer_state_init_model( void );
        //run, returns next state
        virtual renderer_state *run( renderer *r, renderer_writelock *rl, dpthread_lock *thd );

    };

};

#endif
