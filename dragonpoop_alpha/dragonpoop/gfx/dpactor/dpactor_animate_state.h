
#ifndef dragonpoop_dpactor_animate_state_h
#define dragonpoop_dpactor_animate_state_h

#include "dpactor_state.h"
#include "../dpposition/dpposition.h"

namespace dragonpoop
{
    
    class dpactor;
    class dpposition;
    
    class dpactor_animate_state : public dpactor_state
    {
        
    private:
        
        dpactor *a;
        
    protected:
        
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_state **next_state );
        //stop all animations
        void stopAnimations( void );
        //play animation
        void playAnimation( const char *cname, float spd, bool doRepeat );
        //get position
        void getPosition( dpposition *p );

    public:
        
        //ctor
        dpactor_animate_state( dpactor *a );
        //dtor
        virtual ~dpactor_animate_state( void );
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state );
        
    };
    
};

#endif