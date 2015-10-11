
#ifndef dragonpoop_dpactor_model_state_load_high_h
#define dragonpoop_dpactor_model_state_load_high_h

#include "dpactor_model_state.h"

namespace dragonpoop
{
    
    class dpactor;
    class model_loader_ref;
    
    class dpactor_model_state_load_high : public dpactor_model_state
    {
        
    private:
        
        model_loader_ref *ldr;
        
    protected:
        
    public:
        
        //ctor
        dpactor_model_state_load_high( dpactor *a );
        //dtor
        virtual ~dpactor_model_state_load_high( void );
        //load state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif