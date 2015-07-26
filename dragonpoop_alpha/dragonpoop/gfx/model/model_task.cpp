
#include "model_task.h"
#include "model.h"
#include "model_writelock.h"
#include "model_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    model_task::model_task( model *g )
    {
        model_writelock *gl;
        shared_obj_guard sg;

        this->g = 0;
        gl = (model_writelock *)sg.writeLock( g );
        if( !gl )
            return;
        this->g = (model_ref *)gl->getRef();
    }

    //dtor
    model_task::~model_task( void )
    {
        if( this->g )
            delete this->g;
    }

    //run by task
    void model_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        model_writelock *gl;
        shared_obj_guard g;

        if( !this->g )
        {
            tl->kill();
            return;
        }
        gl = (model_writelock *)g.tryWriteLock( this->g, 30 );
        if( !gl )
            return;

        gl->run( th );
    }
    
};
