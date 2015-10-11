
#include "dpactor_model_state_make_low.h"
#include "dpactor_model_state_run_low.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_make_low::dpactor_model_state_make_low( dpactor *a ) : dpactor_model_state( a )
    {
        
    }
    
    //dtor
    dpactor_model_state_make_low::~dpactor_model_state_make_low( void )
    {
        
    }
    
    //make state
    void dpactor_model_state_make_low::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        this->makeLow( thd );
        this->killMed();
        this->killHigh();
        *next_state = new dpactor_model_state_run_low( a );
        std::cout << "low made\r\n";
    }
    
};
