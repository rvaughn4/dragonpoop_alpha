
#ifndef dragonpoop_dpland_man_task_h
#define dragonpoop_dpland_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "dpland_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class dpland_man_ref;
    
    class dpland_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<dpland_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        dpland_man_task( dpland_man *g );
        //dtor
        virtual ~dpland_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif