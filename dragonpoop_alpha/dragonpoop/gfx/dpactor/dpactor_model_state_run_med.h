
#ifndef dragonpoop_dpactor_model_state_run_med_h
#define dragonpoop_dpactor_model_state_run_med_h

#include "dpactor_model_state.h"

namespace dragonpoop
{
    
    class dpactor;
    
    class dpactor_model_state_run_med : public dpactor_model_state
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        dpactor_model_state_run_med( dpactor *a );
        //dtor
        virtual ~dpactor_model_state_run_med( void );
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif