
#ifndef dragonpoop_gui_man_writelock_h
#define dragonpoop_gui_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class gui_man;
    class core;
    class dpthread_lock;
    class gui_ref;
    class gui;
    class gui_factory;
    
    class gui_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        gui_man *t;
        
    protected:
        
        //ctor
        gui_man_writelock( gui_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~gui_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
        void run( dpthread_lock *thd );
        //add gui
        void addGui( gui *g );
        //add gui
        void addGui( gui_ref *g );
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //return gui count
        unsigned int getGuiCount( void );
        //set root gui factory
        void setRootGui( gui_factory *g );

        friend class gui_man;
    };
    
};

#endif