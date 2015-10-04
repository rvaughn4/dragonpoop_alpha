
#ifndef dragonpoop_gui_factory_readlock_h
#define dragonpoop_gui_factory_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class gui;
    class gui_factory;
    class dpthread_lock;
    
    class gui_factory_readlock : public shared_obj_readlock
    {
        
    private:
        
        gui_factory *t;
        
    protected:
        
        //ctor
        gui_factory_readlock( gui_factory *t, dpmutex_readlock *l );
        //dtor
        virtual ~gui_factory_readlock( void );
        
    public:
        
        //make gui
        gui *makeGui( dpthread_lock *thd, gui *p );
        
        friend class gui_factory;
    };
    
};

#endif