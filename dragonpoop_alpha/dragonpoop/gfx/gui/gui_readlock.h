
#ifndef dragonpoop_gui_readlock_h
#define dragonpoop_gui_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    class gui;
    class core;
    struct gui_dims;
    
    class gui_readlock : public shared_obj_readlock
    {
        
    private:
        
        gui *t;
        
    protected:
        
        //ctor
        gui_readlock( gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~gui_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //returns id
        dpid getId( void );
        //compares id
        bool compareId( dpid id );
        //get dimensions
        void getDimensions( gui_dims *p );
        //returns true if has background texture
        bool hasBg( void );
        //returns true if has forground texture
        bool hasFg( void );

        friend class gui;
    };
    
};

#endif