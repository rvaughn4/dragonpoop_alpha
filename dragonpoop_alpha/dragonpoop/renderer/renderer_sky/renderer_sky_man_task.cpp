
#include "renderer_sky_man_task.h"
#include "renderer_sky_man.h"
#include "renderer_sky_man_writelock.h"
#include "renderer_sky_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../renderer.h"
#include "../renderer_ref.h"
#include "../renderer_writelock.h"
#include "../../core/dpthread/dpthread_lock.h"

#include <thread>

namespace dragonpoop
{
    
    //ctor
    renderer_sky_man_task::renderer_sky_man_task( renderer_sky_man *g )
    {
        renderer_sky_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (renderer_sky_man_writelock *)sg.writeLock( g, "renderer_sky_man_task::renderer_sky_man_task" );
        if( !gl )
            return;
        this->g = (renderer_sky_man_ref *)gl->getRef();
    }
    
    //dtor
    renderer_sky_man_task::~renderer_sky_man_task( void )
    {
        renderer_sky_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void renderer_sky_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        shared_obj_guard o;
        renderer_sky_man_writelock *l;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        l = (renderer_sky_man_writelock *)o.tryWriteLock( this->g, 100, "renderer_sky_man_task::run" );
        if( !l )
            return;
        l->run( th );
    }
    
};
