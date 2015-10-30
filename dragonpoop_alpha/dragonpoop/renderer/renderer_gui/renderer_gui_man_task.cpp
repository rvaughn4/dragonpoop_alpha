
#include "renderer_gui_man_task.h"
#include "renderer_gui_man.h"
#include "renderer_gui_man_writelock.h"
#include "renderer_gui_man_ref.h"
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
    renderer_gui_man_task::renderer_gui_man_task( renderer_gui_man *g, renderer *r )
    {
        renderer_gui_man_writelock *gl;
        shared_obj_guard sg;
        renderer_writelock *l;
        
        this->g = 0;
        gl = (renderer_gui_man_writelock *)sg.writeLock( g, "renderer_gui_man_task::renderer_gui_man_task" );
        if( !gl )
            return;
        this->g = (renderer_gui_man_ref *)gl->getRef();
        
        this->r = 0;
        l = (renderer_writelock *)sg.tryWriteLock( r, 5000, "" );
        if( !l )
            return;
        this->r = (renderer_ref *)l->getRef();
    }
    
    //dtor
    renderer_gui_man_task::~renderer_gui_man_task( void )
    {
        renderer_gui_man_ref *gg;
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
    void renderer_gui_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        if( !this->g || !this->r )
        {
            tl->kill();
            return;
        }
        
        renderer_gui_man::run( th, this->g, this->r );
    }
    
};
