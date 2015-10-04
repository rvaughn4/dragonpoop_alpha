
#ifndef dragonpoop_gui_factory_writelock_h
#define dragonpoop_gui_factory_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class gui;
    class gui_factory;
    class dpthread_lock;
    
    class gui_factory_writelock : public shared_obj_writelock
    {
        
    private:
        
        gui_factory *t;
        
    protected:
        
        //ctor
        gui_factory_writelock( gui_factory *t, dpmutex_writelock *l );
        //dtor
        virtual ~gui_factory_writelock( void );
        
    public:
        
        //make gui
        gui *makeGui( dpthread_lock *thd, gui *p );
        
        friend class gui_factory;
    };
    
};

#endif