
#include "dpactor_man_task.h"
#include "dpactor_man.h"
#include "dpactor_man_writelock.h"
#include "dpactor_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_man_task::dpactor_man_task( dpactor_man *g )
    {
        dpactor_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (dpactor_man_writelock *)sg.writeLock( g, "dpactor_man_task::dpactor_man_task" );
        if( !gl )
            return;
        this->g = (dpactor_man_ref *)gl->getRef();
    }
    
    //dtor
    dpactor_man_task::~dpactor_man_task( void )
    {
        dpactor_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void dpactor_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        dpactor_man_writelock *gl;
        shared_obj_guard g;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        gl = (dpactor_man_writelock *)g.tryWriteLock( this->g, 100, "dpactor_man_task::run" );
        if( gl )
            gl->run( th );
        g.unlock();
    }
    
};
