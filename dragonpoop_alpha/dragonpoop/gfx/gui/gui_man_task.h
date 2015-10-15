
#ifndef dragonpoop_gui_man_task_h
#define dragonpoop_gui_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "gui_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class gui_man_ref;
    
    class gui_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<gui_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        gui_man_task( gui_man *g );
        //dtor
        virtual ~gui_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif