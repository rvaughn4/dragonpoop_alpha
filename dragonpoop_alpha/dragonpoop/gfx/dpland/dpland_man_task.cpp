
#include "dpland_man_task.h"
#include "dpland_man.h"
#include "dpland_man_writelock.h"
#include "dpland_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    dpland_man_task::dpland_man_task( dpland_man *g )
    {
        dpland_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (dpland_man_writelock *)sg.writeLock( g, "dpland_man_task::dpland_man_task" );
        if( !gl )
            return;
        this->g = (dpland_man_ref *)gl->getRef();
    }
    
    //dtor
    dpland_man_task::~dpland_man_task( void )
    {
        dpland_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void dpland_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        dpland_man_writelock *gl;
        shared_obj_guard g;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        gl = (dpland_man_writelock *)g.tryWriteLock( this->g, 100, "dpland_man_task::run" );
        if( gl )
            gl->run( th );
        g.unlock();
    }
    
};
