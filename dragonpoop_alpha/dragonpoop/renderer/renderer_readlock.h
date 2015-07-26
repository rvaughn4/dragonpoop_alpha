
#ifndef dragonpoop_renderer_readlock_h
#define dragonpoop_renderer_readlock_h

#include "../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class renderer;
    class core;

    class renderer_readlock : public shared_obj_readlock
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_readlock( renderer *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_readlock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );

        friend class renderer;
    };
    
};

#endif