
#include "model_loader_state_close.h"
#include "model_loader_writelock.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_state_close::model_loader_state_close( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_state_close::~model_loader_state_close( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state_close::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        ml->t->bIsRun = 0;
        if( this->b )
            delete b;
        this->b = 0;
        ml->unlink();
        return 0;
    }
    
};
