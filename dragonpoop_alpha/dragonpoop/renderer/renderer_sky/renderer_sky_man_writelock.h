
#ifndef dragonpoop_renderer_sky_man_writelock_h
#define dragonpoop_renderer_sky_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class renderer_sky_man;
    class core;
    class dpthread_lock;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class dpmatrix;
    
    class renderer_sky_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_sky_man *t;
        
    protected:
        
        //ctor
        renderer_sky_man_writelock( renderer_sky_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_sky_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run from manager thread
        void run( dpthread_lock *thd );
        
        friend class renderer_sky_man;
    };
    
};

#endif