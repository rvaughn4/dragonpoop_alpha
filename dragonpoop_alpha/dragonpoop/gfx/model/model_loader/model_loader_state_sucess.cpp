
#include "model_loader_state_sucess.h"
#include "model_loader_writelock.h"
#include "model_loader_state_close.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_state_sucess::model_loader_state_sucess( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_state_sucess::~model_loader_state_sucess( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state_sucess::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        ml->t->bWasOpen = 1;
        return new model_loader_state_close( this->b );
    }
    
};
