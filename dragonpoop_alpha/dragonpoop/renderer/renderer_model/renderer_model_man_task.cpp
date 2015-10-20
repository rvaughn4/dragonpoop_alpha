
#include "renderer_model_man_task.h"
#include "renderer_model_man.h"
#include "renderer_model_man_writelock.h"
#include "renderer_model_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man_task::renderer_model_man_task( renderer_model_man *g )
    {
        renderer_model_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (renderer_model_man_writelock *)sg.writeLock( g, "renderer_model_man_task::renderer_model_man_task" );
        if( !gl )
            return;
        this->g = (renderer_model_man_ref *)gl->getRef();
    }
    
    //dtor
    renderer_model_man_task::~renderer_model_man_task( void )
    {
        renderer_model_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void renderer_model_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        renderer_model_man::runFromTask( th, this->g );
    }
    
};
