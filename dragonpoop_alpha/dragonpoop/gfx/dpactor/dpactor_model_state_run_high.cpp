
#include "dpactor_model_state_run_high.h"
#include "dpactor_model_state_load_high.h"
#include "dpactor_model_state_run_med.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_run_high::dpactor_model_state_run_high( dpactor *a ) : dpactor_model_state( a )
    {
        
    }
    
    //dtor
    dpactor_model_state_run_high::~dpactor_model_state_run_high( void )
    {
        
    }
    
    //run state
    void dpactor_model_state_run_high::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        float d;
        
        d = this->getDistance();
        *next_state = 0;
        
        if( d > 0.5f )
        {
            *next_state = new dpactor_model_state_run_med( a );
            return;
        }
        
        if( !this->isHigh() )
        {
            *next_state = new dpactor_model_state_load_high( a );
            return;
        }
    }
    
};
