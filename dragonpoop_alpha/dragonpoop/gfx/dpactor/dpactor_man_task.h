
#ifndef dragonpoop_dpactor_man_task_h
#define dragonpoop_dpactor_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "dpactor_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class dpactor_man_ref;
    
    class dpactor_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<dpactor_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        dpactor_man_task( dpactor_man *g );
        //dtor
        virtual ~dpactor_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif