
#include "model_loader_state_sucess.h"
#include "model_loader_writelock.h"
#include "model_loader_state_close.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../model_ref.h"
#include "../model_writelock.h"

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
        model_ref *mr;
        model_writelock *m;
        shared_obj_guard g;
        
        ml->t->bWasOpen = 1;
        mr = ml->getModel();
        if( mr )
        {
            m = (model_writelock *)g.tryWriteLock( mr, 1000, "model_loader_state_sucess::run" );
            if( m )
                m->sync( thd );
            delete mr;
        }
        
        return new model_loader_state_close( this->b );
    }
    
};
