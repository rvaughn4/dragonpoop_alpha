
#ifndef dragonpoop_gfx_readlock_h
#define dragonpoop_gfx_readlock_h

#include "../core/shared_obj/shared_obj_readlock.h"
#include <list>

namespace dragonpoop
{
    class gfx;
    class core;

    class gfx_readlock : public shared_obj_readlock
    {

    private:

        gfx *t;

    protected:

        //ctor
        gfx_readlock( gfx *t, dpmutex_readlock *l );
        //dtor
        virtual ~gfx_readlock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        
        friend class gfx;
    };
    
};

#endif