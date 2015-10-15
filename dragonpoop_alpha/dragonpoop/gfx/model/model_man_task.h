
#ifndef dragonpoop_model_man_task_h
#define dragonpoop_model_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "model_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class model_man_ref;
    
    class model_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<model_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        model_man_task( model_man *g );
        //dtor
        virtual ~model_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif