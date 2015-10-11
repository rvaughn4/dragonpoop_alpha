
#ifndef dragonpoop_dpactor_model_state_load_med_h
#define dragonpoop_dpactor_model_state_load_med_h

#include "dpactor_model_state.h"

namespace dragonpoop
{
    
    class dpactor;
    class model_loader_ref;
    
    class dpactor_model_state_load_med : public dpactor_model_state
    {
        
    private:
        
        model_loader_ref *ldr;
        
    protected:
        
    public:
        
        //ctor
        dpactor_model_state_load_med( dpactor *a );
        //dtor
        virtual ~dpactor_model_state_load_med( void );
        //load state
        virtual void run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state );
        
    };
    
};

#endif