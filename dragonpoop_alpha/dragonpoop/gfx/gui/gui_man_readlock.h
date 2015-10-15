
#ifndef dragonpoop_gui_man_readlock_h
#define dragonpoop_gui_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class gui_man;
    class core;
    class gui_ref;
    class gui;
    
    class gui_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        gui_man *t;
        
    protected:
        
        //ctor
        gui_man_readlock( gui_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~gui_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //return gui count
        unsigned int getGuiCount( void );

        friend class gui_man;
    };
    
};

#endif