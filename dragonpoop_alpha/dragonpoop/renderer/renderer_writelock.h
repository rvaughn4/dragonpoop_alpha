
#ifndef dragonpoop_renderer_writelock_h
#define dragonpoop_renderer_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class renderer;
    class core;
    class dpthread_lock;
    class dptask_writelock;
    class gfx_writelock;
    class renderer_writelock;

    class renderer_writelock : public shared_obj_writelock
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_writelock( renderer *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_writelock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //stop renderer task
        void kill( void );
        //run renderer from task
        void run( dptask_writelock *tskl, dpthread_lock *thd );

        friend class renderer;
    };
    
};

#endif