
#ifndef dragonpoop_renderer_state_start_gui_h
#define dragonpoop_renderer_state_start_gui_h

#include "renderer_state.h"

namespace dragonpoop
{
    
    class renderer_state_start_gui : public renderer_state
    {
        
    private:
        
    protected:
        
        
    public:
        
        //ctor
        renderer_state_start_gui( renderer *r );
        //dtor
        virtual ~renderer_state_start_gui( void );
        //run, returns next state
        virtual renderer_state *run( renderer *r, renderer_writelock *rl, dpthread_lock *thd );
        
    };
    
};

#endif