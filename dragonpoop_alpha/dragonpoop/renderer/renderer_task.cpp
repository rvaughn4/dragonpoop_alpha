
#include "renderer_task.h"
#include "renderer.h"
#include "renderer_writelock.h"
#include "renderer_ref.h"
#include "../core/dptask/dptask_writelock.h"
#include "../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    renderer_task::renderer_task( renderer *g )
    {
        renderer_writelock *gl;
        shared_obj_guard lg;

        this->g = 0;
        gl = (renderer_writelock *)lg.writeLock( g );
        if( !gl )
            return;
        this->g = (renderer_ref *)gl->getRef();
    }

    //dtor
    renderer_task::~renderer_task( void )
    {
        if( this->g )
            delete this->g;
    }

    //run by task
    void renderer_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        renderer_writelock *gl;
        shared_obj_guard lg;

        if( !this->g )
        {
            tl->kill();
            return;
        }
        gl = (renderer_writelock *)lg.tryWriteLock( this->g, 30 );
        if( !gl )
            return;

        gl->run( tl, th );
    }
    
};
