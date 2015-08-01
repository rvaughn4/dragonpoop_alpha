
#include "model_loader_state.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_state::model_loader_state( void )
    {
        
    }
    
    //dtor
    model_loader_state::~model_loader_state( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        return 0;
    }
    
};
