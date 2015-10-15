
#include "model_man_task.h"
#include "model_man.h"
#include "model_man_writelock.h"
#include "model_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    model_man_task::model_man_task( model_man *g )
    {
        model_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (model_man_writelock *)sg.writeLock( g, "model_man_task::model_man_task" );
        if( !gl )
            return;
        this->g = (model_man_ref *)gl->getRef();
    }
    
    //dtor
    model_man_task::~model_man_task( void )
    {
        model_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void model_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        model_man_writelock *gl;
        shared_obj_guard g;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        gl = (model_man_writelock *)g.tryWriteLock( this->g, 100, "model_man_task::run" );
        if( gl )
            gl->run( th );
        g.unlock();
    }
    
};
