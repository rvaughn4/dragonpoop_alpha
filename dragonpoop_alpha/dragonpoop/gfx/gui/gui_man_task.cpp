
#include "gui_man_task.h"
#include "gui_man.h"
#include "gui_man_writelock.h"
#include "gui_man_ref.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    gui_man_task::gui_man_task( gui_man *g )
    {
        gui_man_writelock *gl;
        shared_obj_guard sg;
        
        this->g = 0;
        gl = (gui_man_writelock *)sg.writeLock( g, "gui_man_task::gui_man_task" );
        if( !gl )
            return;
        this->g = (gui_man_ref *)gl->getRef();
    }
    
    //dtor
    gui_man_task::~gui_man_task( void )
    {
        gui_man_ref *gg;
        if( this->g )
        {
            gg = this->g;
            delete gg;
        }
        this->g = 0;
    }
    
    //run by task
    void gui_man_task::run( dptask_writelock *tl, dpthread_lock *th )
    {
        gui_man_writelock *gl;
        shared_obj_guard g;
        
        if( !this->g )
        {
            tl->kill();
            return;
        }
        
        gl = (gui_man_writelock *)g.tryWriteLock( this->g, 100, "gui_man_task::run" );
        if( gl )
            gl->run( th );
        g.unlock();
    }
    
};
