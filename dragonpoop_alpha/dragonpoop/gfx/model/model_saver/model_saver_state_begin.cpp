
#include "model_saver_state_begin.h"
#include "model_saver_writelock.h"
#include <fstream>
#include "model_saver_state_fail.h"
#include "../../../core/dpbuffer/dpbuffer_dynamic.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_state_begin::model_saver_state_begin( void )
    {
        
    }
    
    //dtor
    model_saver_state_begin::~model_saver_state_begin( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_state_begin::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        dpbuffer *b;
        
        b = new dpbuffer_dynamic();
        if( !b )
            return new model_saver_state_fail( 0 );
        
        return ml->t->genState( b );
    }
    
};
