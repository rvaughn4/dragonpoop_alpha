
#include "model_saver_task.h"
#include "model_saver_writelock.h"
#include "model_saver_ref.h"
#include "model_saver.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_task::model_saver_task( model_saver *l )
    {
        shared_obj_guard o;
        model_saver_writelock *ll;
        
        ll = (model_saver_writelock *)o.writeLock( l );
        this->l = (model_saver_ref *)ll->getRef();
    }
    
    //dtor
    model_saver_task::~model_saver_task( void )
    {
        
    }
    
    //run by task
    void model_saver_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        shared_obj_guard o;
        model_saver_writelock *ll;
        
        ll = (model_saver_writelock *)o.tryWriteLock( this->l, 100 );
        if( !ll )
            return;
        ll->run( th );
    }
    
};
