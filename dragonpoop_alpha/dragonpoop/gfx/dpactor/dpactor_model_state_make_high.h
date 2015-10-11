
#ifndef dragonpoop_dpactor_model_state_make_high_h
#define dragonpoop_dpactor_model_state_make_high_h

#include "dpactor_model_state.h"

namespace dragonpoop
{
    
    class dpactor;
    
    class dpactor_model_state_make_high : public dpactor_model_state
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        dpactor_model_state_make_high( dpactor *a );
        //dtor
        virtual ~dpactor_model_state_make_high( void );
        //make state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif