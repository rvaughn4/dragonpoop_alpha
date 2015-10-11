
#include "dpactor_model_state_run_low.h"
#include "dpactor_model_state_load_low.h"

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
    void dpactor_model_state_run_low::run( dpactor *a, dpactor_model_state **next_state )
    {
        float d;
        
        d = this->getDistance();
        
        if( d < 20.0f )
        {
            *next_state = 0; //load med
            return;
        }
        
        if( !this->isLow() )
        {
            *next_state = new dpactor_model_state_load_low( a );
            return;
        }
    }

};
