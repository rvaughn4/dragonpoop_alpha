
#ifndef dragonpoop_renderer_gui_man_writelock_h
#define dragonpoop_renderer_gui_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class renderer_gui_man;
    class core;
    class dpthread_lock;
    class renderer_gui_ref;
    class renderer_gui;
    
    class renderer_gui_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_gui_man *t;
        
    protected:
        
        //ctor
        renderer_gui_man_writelock( renderer_gui_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_gui_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run from manager thread
        void runFromTask( dpthread_lock *thd );
        //run from renderer thread
        void runFromRenderer( dpthread_lock *thd );
        
        friend class renderer_gui_man;
    };
    
};

#endif