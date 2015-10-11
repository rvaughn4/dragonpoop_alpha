
#include "dpactor_state.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_state::dpactor_state( dpactor *a )
    {
        
    }
    
    //dtor
    dpactor_state::~dpactor_state( void )
    {
        
    }
    
    //run state
    void dpactor_state::run( dpactor *a, dpactor_state **next_state )
    {
        *next_state = 0;
    }
    
};
