
#ifndef dragonpoop_dpsky_man_task_h
#define dragonpoop_dpsky_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "dpsky_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class dpsky_man_ref;
    
    class dpsky_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<dpsky_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        dpsky_man_task( dpsky_man *g );
        //dtor
        virtual ~dpsky_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif