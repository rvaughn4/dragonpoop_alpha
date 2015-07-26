
#ifndef dragonpoop_renderer_task_h
#define dragonpoop_renderer_task_h

#include "../core/dptask/dptask_owner.h"
#include "renderer.h"

namespace dragonpoop
{

    class renderer_ref;

    class renderer_task : public dptask_owner
    {

    private:

        renderer_ref *g;

    protected:

    public:

        //ctor
        renderer_task( renderer *g );
        //dtor
        virtual ~renderer_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );
        
    };
    
};

#endif