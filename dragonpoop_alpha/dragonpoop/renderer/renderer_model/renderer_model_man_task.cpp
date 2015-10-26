
#include "renderer_model_man_task.h"
#include "renderer_model_man.h"
#include "renderer_model_man_writelock.h"
#include "renderer_model_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../renderer.h"
#include "../renderer_ref.h"
#include "../renderer_writelock.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man_task::renderer_model_man_task( renderer_model_man *g, renderer *r )
    {
        renderer_model_man_writelock *gl;
        shared_obj_guard sg;
        renderer_writelock *l;
        
        this->r = 0;
        this->g = 0;
        gl = (renderer_model_man_writelock *)sg.writeLock( g, "renderer_model_man_task::renderer_model_man_task" );
        if( !gl )
            return;
        this->g = (renderer_model_man_ref *)gl->getRef();
        l = (renderer_writelock *)sg.writeLock( r, "renderer_model_man_task::renderer_model_man_task" );
        if( !l )
            return;
        this->r = (renderer_ref *)l->getRef();
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
        if( this->r )
            delete this->r;
    }
    
    //run by task
    void renderer_model_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        if( !this->g || !this->r )
        {
            tl->kill();
            return;
        }
        
        renderer_model_man::run( th, this->g, this->r );
    }
    
};
