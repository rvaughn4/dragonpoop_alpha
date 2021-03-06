
#include "dpactor_model_state_run_low.h"
#include "dpactor_model_state_load_low.h"
#include "dpactor_model_state_run_med.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_run_low::dpactor_model_state_run_low( dpactor *a ) : dpactor_model_state( a )
    {
        
    }
    
    //dtor
    dpactor_model_state_run_low::~dpactor_model_state_run_low( void )
    {
        
    }
    
    //run state
    void dpactor_model_state_run_low::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        float d;
        
        d = this->getDistance();
        *next_state = 0;
        
        if( d < 10.0f )
        {
            *next_state = new dpactor_model_state_run_med( a );
            return;
        }
        
        if( !this->isLow() )
        {
            *next_state = new dpactor_model_state_load_low( a );
            return;
        }
    }

};
