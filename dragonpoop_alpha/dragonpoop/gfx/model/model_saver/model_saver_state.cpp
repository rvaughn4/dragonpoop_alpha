
#include "model_saver_state.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_state::model_saver_state( void )
    {
        
    }
    
    //dtor
    model_saver_state::~model_saver_state( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_state::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        return 0;
    }
    
};
