
#ifndef dragonpoop_renderer_land_man_task_h
#define dragonpoop_renderer_land_man_task_h

#include "../../core/dptask/dptask_owner.h"
#include "renderer_land_man.h"
#include <atomic>

namespace dragonpoop
{
    
    class renderer_land_man_ref;
    
    class renderer_land_man_task : public dptask_owner
    {
        
    private:
        
        std::atomic<renderer_land_man_ref *> g;
        
    protected:
        
    public:
        
        //ctor
        renderer_land_man_task( renderer_land_man *g );
        //dtor
        virtual ~renderer_land_man_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif