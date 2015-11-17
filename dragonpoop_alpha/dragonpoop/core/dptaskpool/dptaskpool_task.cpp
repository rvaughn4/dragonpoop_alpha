
#include "dptaskpool_task.h"
#include "dptaskpool.h"
#include "dptaskpool_writelock.h"
#include "dptaskpool_ref.h"
#include "../dptask/dptask_writelock.h"
#include "../shared_obj/shared_obj_guard.h"
#include "../dpthread/dpthread_lock.h"

#include <thread>

namespace dragonpoop
{

    //ctor
    dptaskpool_task::dptaskpool_task( dptaskpool *g )
    {
        dptaskpool_writelock *gl;
        shared_obj_guard lg;

        this->g = 0;
        gl = (dptaskpool_writelock *)lg.writeLock( g, "dptaskpool_task::dptaskpool_task" );
        if( !gl )
            return;
        this->g = (dptaskpool_ref *)gl->getRef();
    }

    //dtor
    dptaskpool_task::~dptaskpool_task( void )
    {
        dptaskpool_ref *gg;

        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }

    //run by task
    void dptaskpool_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        dptaskpool_writelock *gl;
        shared_obj_guard lg;

        if( !this->g )
        {
            tl->kill();
            return;
        }

        gl = (dptaskpool_writelock *)lg.tryWriteLock( this->g, 30, "dptaskpool_task::run" );
        if( !gl )
            return;

        gl->run( tl, th );

    }

};
