
#include "gfx_task.h"
#include "gfx.h"
#include "gfx_writelock.h"
#include "gfx_ref.h"
#include "../core/dptask/dptask_writelock.h"
#include "../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    gfx_task::gfx_task( gfx *g )
    {
        gfx_writelock *gl;
        shared_obj_guard sg;

        this->g = 0;
        gl = (gfx_writelock *)sg.writeLock( g, "gfx_task::gfx_task" );
        if( !gl )
            return;
        this->g = (gfx_ref *)gl->getRef();
    }

    //dtor
    gfx_task::~gfx_task( void )
    {
        gfx_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }

    //run by task
    void gfx_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        gfx_writelock *gl;
        shared_obj_guard g;

        if( !this->g )
        {
            tl->kill();
            return;
        }
        gl = (gfx_writelock *)g.tryWriteLock( this->g, 100, "gfx_task::run" );
        if( !gl )
            return;

        gl->run( th );
        g.unlock();
        
        gfx::runModels( th, this->g );
        gfx::runGuis( th, this->g );
        gfx::runActors( th, this->g );
    }
    
};
