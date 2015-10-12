
#ifndef dragonpoop_dpactor_animate_state_fly_h
#define dragonpoop_dpactor_animate_state_fly_h

#include "dpactor_animate_state.h"
#include <stdint.h>

namespace dragonpoop
{
    
    class dpactor;
    
    class dpactor_animate_state_fly : public dpactor_animate_state
    {
        
        uint64_t t_start;
        
    private:
        
    protected:
        
    public:
        
        //ctor
        dpactor_animate_state_fly( dpactor *a );
        //dtor
        virtual ~dpactor_animate_state_fly( void );
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state );
        
    };
    
};

#endif