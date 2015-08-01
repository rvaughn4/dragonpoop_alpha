
#ifndef dragonpoop_model_loader_state_h
#define dragonpoop_model_loader_state_h

#include "model_loader.h"

namespace dragonpoop
{
    
    class model_loader_state
    {
        
    private:
        
    protected:
        
        //ctor
        model_loader_state( void );
        
    public:
        
        //dtor
        virtual ~model_loader_state( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif