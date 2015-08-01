
#include "model_loader_task.h"
#include "model_loader_writelock.h"
#include "model_loader_ref.h"
#include "model_loader.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_task::model_loader_task( model_loader *l )
    {
        shared_obj_guard o;
        model_loader_writelock *ll;

        ll = (model_loader_writelock *)o.writeLock( l );
        this->l = (model_loader_ref *)ll->getRef();
    }
    
    //dtor
    model_loader_task::~model_loader_task( void )
    {
        
    }
    
    //run by task
    void model_loader_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        shared_obj_guard o;
        model_loader_writelock *ll;
        
        ll = (model_loader_writelock *)o.tryWriteLock( this->l, 100 );
        if( !ll )
            return;
        ll->run( th );
    }

};
