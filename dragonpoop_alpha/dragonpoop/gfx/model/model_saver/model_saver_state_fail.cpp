
#include "model_saver_state_fail.h"
#include "model_saver_writelock.h"
#include "../../../core/dpbuffer/dpbuffer_dynamic.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_state_fail::model_saver_state_fail( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_saver_state_fail::~model_saver_state_fail( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_state_fail::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ml->t->bWasOpen = 0;
        ml->t->bIsRun = 0;
        
        if( this->b )
            delete this->b;
        this->b = 0;
        
        return 0;
    }
    
};
