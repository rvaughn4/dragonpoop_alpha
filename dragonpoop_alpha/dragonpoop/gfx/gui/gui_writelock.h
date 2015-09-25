
#ifndef dragonpoop_gui_writelock_h
#define dragonpoop_gui_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    class gui;
    class core;
    
    class gui_writelock : public shared_obj_writelock
    {
        
    private:
        
        gui *t;
        
    protected:
        
        //ctor
        gui_writelock( gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~gui_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //returns id
        dpid getId( void );
        //compares id
        bool compareId( dpid id );

        friend class gui;
    };
    
};

#endif