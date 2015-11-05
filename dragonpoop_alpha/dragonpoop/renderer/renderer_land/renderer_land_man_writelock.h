
#ifndef dragonpoop_renderer_land_man_writelock_h
#define dragonpoop_renderer_land_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class renderer_land_man;
    class core;
    class dpthread_lock;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class dpmatrix;
    
    class renderer_land_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_land_man *t;
        
    protected:
        
        //ctor
        renderer_land_man_writelock( renderer_land_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_land_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //render lands
        void renderLands( dpthread_lock *thd, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *cl );
        //run from manager thread
        void run( dpthread_lock *thd );

        friend class renderer_land_man;
    };
    
};

#endif