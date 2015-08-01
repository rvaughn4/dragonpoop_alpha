
#ifndef dragonpoop_model_loader_state_sucess_h
#define dragonpoop_model_loader_state_sucess_h

#include "model_loader_state.h"

namespace dragonpoop
{
    
    class model_loader_state_sucess : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
    public:
        
        //ctor
        model_loader_state_sucess( dpbuffer *b );
        //dtor
        virtual ~model_loader_state_sucess( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif