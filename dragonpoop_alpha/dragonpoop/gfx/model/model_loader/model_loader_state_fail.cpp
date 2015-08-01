
#include "model_loader_state_fail.h"
#include "model_loader_writelock.h"
#include "model_loader_state_close.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_state_fail::model_loader_state_fail( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_state_fail::~model_loader_state_fail( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state_fail::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        ml->t->bWasOpen = 0;
        return new model_loader_state_close( this->b );
    }
    
};
