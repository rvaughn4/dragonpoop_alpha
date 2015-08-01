
#ifndef dragonpoop_model_loader_state_fail_h
#define dragonpoop_model_loader_state_fail_h

#include "model_loader_state.h"

namespace dragonpoop
{
    
    class model_loader_state_fail : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_loader_state_fail( dpbuffer *b );
        //dtor
        virtual ~model_loader_state_fail( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif