
#ifndef dragonpoop_gfx_task_h
#define dragonpoop_gfx_task_h

#include "../core/dptask/dptask_owner.h"
#include "gfx.h"
#include <atomic>

namespace dragonpoop
{

    class gfx_ref;

    class gfx_task : public dptask_owner
    {

    private:

        std::atomic<gfx_ref *> g;
        bool bRunGfx, bRunModels, bRunGuis, bRunActors;

    protected:

    public:

        //ctor
        gfx_task( gfx *g, bool bRunGfx, bool bRunModels, bool bRunGuis, bool bRunActors );
        //dtor
        virtual ~gfx_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif