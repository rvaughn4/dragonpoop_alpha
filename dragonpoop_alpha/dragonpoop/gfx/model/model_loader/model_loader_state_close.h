
#ifndef dragonpoop_model_loader_state_close_h
#define dragonpoop_model_loader_state_close_h

#include "model_loader_state.h"

namespace dragonpoop
{
    
    class model_loader_state_close : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        
    protected:
        
        //ctor
        model_loader_state_close( dpbuffer *b );
        
    public:
        
        //dtor
        virtual ~model_loader_state_close( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
        friend class model_loader_state_sucess;
        friend class model_loader_state_fail;
    };
    
};

#endif