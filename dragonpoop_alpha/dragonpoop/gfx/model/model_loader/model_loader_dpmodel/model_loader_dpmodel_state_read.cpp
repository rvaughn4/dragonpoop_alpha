
#include "model_loader_dpmodel_state_read.h"
#include "../model_loader_writelock.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_state_sucess.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../model_component/model_component.h"
#include <list>

namespace dragonpoop
{
    
    //ctor
    model_loader_dpmodel_state_read::model_loader_dpmodel_state_read( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_dpmodel_state_read::~model_loader_dpmodel_state_read( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_dpmodel_state_read::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e;
        
        if( !this->m )
            return new model_loader_state_fail( this->b );
        m = (model_writelock *)o.tryWriteLock( this->m, 300, "model_loader_dpmodel_state_read::run" );
        if( !m )
            return new model_loader_state_fail( this->b );
        
        if( !m->readHeader( this->b, &e ) )
            return new model_loader_state_fail( this->b );
        
        for( i = 0; i < e; i++ )
            m->readComponent( this->b, 0 );
        
        o.unlock();
        return new model_loader_state_sucess( this->b );
    }
    
};
