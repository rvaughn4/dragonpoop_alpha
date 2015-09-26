
#ifndef dragonpoop_renderer_gui_readlock_h
#define dragonpoop_renderer_gui_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    class renderer_gui;
    class core;
    struct gui_dims;
    
    class renderer_gui_readlock : public shared_obj_readlock
    {
        
    private:
        
        renderer_gui *t;
        
    protected:
        
        //ctor
        renderer_gui_readlock( renderer_gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_gui_readlock( void );
        
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
        //get parent id
        dpid getParentId( void );
        //compares parent id
        bool compareParentId( dpid id );
        
        friend class gui;
    };
    
};

#endif