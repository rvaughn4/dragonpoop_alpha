
#include "dpactor_model_state_run_med.h"
#include "dpactor_model_state_load_med.h"
#include "dpactor_model_state_run_low.h"
#include "dpactor_model_state_run_high.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_run_med::dpactor_model_state_run_med( dpactor *a ) : dpactor_model_state( a )
    {
        
    }
    
    //dtor
    dpactor_model_state_run_med::~dpactor_model_state_run_med( void )
    {
        
    }
    
    //run state
    void dpactor_model_state_run_med::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        float d;
        
        d = this->getDistance();
        *next_state = 0;
        
        if( d > 10.0f )
        {
            *next_state = new dpactor_model_state_run_low( a );
            return;
        }
        if( d < 5.0f )
        {
            *next_state = new dpactor_model_state_run_high( a );
            return;
        }
        
        if( !this->isMed() )
        {
            *next_state = new dpactor_model_state_load_med( a );
            return;
        }
    }
    
};
