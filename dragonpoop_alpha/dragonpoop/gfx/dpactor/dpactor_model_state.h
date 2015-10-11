
#ifndef dragonpoop_dpactor_model_state_h
#define dragonpoop_dpactor_model_state_h

#include "dpactor_state.h"

namespace dragonpoop
{
    
    class dpactor;
    class model_loader_ref;
    
    class dpactor_model_state : public dpactor_state
    {
        
    private:
        
        dpactor *a;
        
    protected:
        
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_state **next_state );
        //return camera distance
        float getDistance( void );
        //returns true if low is created
        bool isLow( void );
        //load low model
        bool loadLow( model_loader_ref **ldr );
        //make low model instance
        bool makeLow( dpthread_lock *thd );

    public:
        
        //ctor
        dpactor_model_state( dpactor *a );
        //dtor
        virtual ~dpactor_model_state( void );
        //run state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif