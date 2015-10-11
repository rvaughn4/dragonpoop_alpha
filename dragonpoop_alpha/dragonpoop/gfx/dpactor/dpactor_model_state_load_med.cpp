
#include "dpactor_model_state_load_med.h"
#include "dpactor_model_state_make_med.h"
#include "../model/model_loader/model_loader_ref.h"
#include "../model/model_loader/model_loader_readlock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state_load_med::dpactor_model_state_load_med( dpactor *a ) : dpactor_model_state( a )
    {
        this->loadMed( &this->ldr );
    }
    
    //dtor
    dpactor_model_state_load_med::~dpactor_model_state_load_med( void )
    {
        
    }
    
    //load state
    void dpactor_model_state_load_med::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        shared_obj_guard o;
        model_loader_readlock *l;
        
        if( !this->ldr || !this->ldr->isLinked() )
        {
            *next_state = new dpactor_model_state_make_med( a );
            return;
        }
        
        *next_state = 0;
        l = (model_loader_readlock *)o.tryReadLock( this->ldr, 100, "dpactor_model_state_load_med::load" );
        if( !l )
            return;
        if( l->isRunning() )
            return;
        
        *next_state = new dpactor_model_state_make_med( a );
    }
    
};
