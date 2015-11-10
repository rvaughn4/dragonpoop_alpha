
#include "dpsky_man_task.h"
#include "dpsky_man.h"
#include "dpsky_man_writelock.h"
#include "dpsky_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    dpsky_man_task::dpsky_man_task( dpsky_man *g )
    {
        dpsky_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (dpsky_man_writelock *)sg.writeLock( g, "dpsky_man_task::dpsky_man_task" );
        if( !gl )
            return;
        this->g = (dpsky_man_ref *)gl->getRef();
    }
    
    //dtor
    dpsky_man_task::~dpsky_man_task( void )
    {
        dpsky_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void dpsky_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        dpsky_man_writelock *gl;
        shared_obj_guard g;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        gl = (dpsky_man_writelock *)g.tryWriteLock( this->g, 100, "dpsky_man_task::run" );
        if( gl )
            gl->run( th );
        g.unlock();
    }
    
};
