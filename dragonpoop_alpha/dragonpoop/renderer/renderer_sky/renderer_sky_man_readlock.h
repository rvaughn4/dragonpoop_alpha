
#ifndef dragonpoop_renderer_sky_man_readlock_h
#define dragonpoop_renderer_sky_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    
    class renderer_sky_man;
    class core;
    class dpthread_lock;
    
    class renderer_sky_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        renderer_sky_man *t;
        
    protected:
        
        //ctor
        renderer_sky_man_readlock( renderer_sky_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_sky_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        
        friend class renderer_sky_man;
    };
    
};

#endif