
#include "dpactor_model_state_make_med.h"
#include "dpactor_model_state_run_med.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_make_med::dpactor_model_state_make_med( dpactor *a ) : dpactor_model_state( a )
    {
        
    }
    
    //dtor
    dpactor_model_state_make_med::~dpactor_model_state_make_med( void )
    {
        
    }
    
    //make state
    void dpactor_model_state_make_med::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        this->makeMed( thd );
        this->killLow();
        this->killHigh();
        *next_state = new dpactor_model_state_run_med( a );
        std::cout << "medium made\r\n";
    }
    
};
