
#ifndef dragonpoop_dpactor_model_state_make_low_h
#define dragonpoop_dpactor_model_state_make_low_h

#include "dpactor_model_state.h"

namespace dragonpoop
{
    
    class dpactor;
    
    class dpactor_model_state_make_low : public dpactor_model_state
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        dpactor_model_state_make_low( dpactor *a );
        //dtor
        virtual ~dpactor_model_state_make_low( void );
        //make state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif