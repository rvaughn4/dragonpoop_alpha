
#ifndef dragonpoop_gui_writelock_h
#define dragonpoop_gui_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    class gui;
    class core;
    struct gui_dims;
    class dpthread_lock;
    
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
        
        //run gui
        void run( dpthread_lock *thd );
        //return core
        core *getCore( void );
        //returns id
        dpid getId( void );
        //compares id
        bool compareId( dpid id );
        //set width and height
        void setWidthHeight( float w, float h );
        //set top left pos
        void setPosition( float x, float y );
        //get dimensions
        void getDimensions( gui_dims *p );
        //cause redraw of background and forground texture
        void redraw( void );
        //set background redraw mode
        void enableBgRedraw( bool b );
        //set forground redraw mode
        void enableFgRedraw( bool b );
        //set background mode
        void enableBg( bool b );
        //set forground mode
        void enableFg( bool b );
        //returns true if has background texture
        bool hasBg( void );
        //returns true if has forground texture
        bool hasFg( void );
        //set parent id
        void setParentId( dpid id );
        //get parent id
        dpid getParentId( void );

        friend class gui;
    };
    
};

#endif