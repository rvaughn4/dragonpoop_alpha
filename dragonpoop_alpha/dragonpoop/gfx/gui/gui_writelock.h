
#ifndef dragonpoop_gui_writelock_h
#define dragonpoop_gui_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{
    class gui;
    class core;
    struct gui_dims;
    class dpthread_lock;
    class dpbitmap;
    class gfx_readlock;
    class gfx_writelock;
    class renderer_gui;
    class gfx_writelock;
    
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
        //returns pointer to bg texture
        dpbitmap *getBg( void );
        //returns pointer to fg texture
        dpbitmap *getFg( void );
        //returns z order
        unsigned int getZ( void );
        //sets focus
        void setFocus( gfx_readlock *g );
        //sets focus
        void setFocus( gfx_writelock *g );
        //returns true if has focus
        bool hasFocus( void );
        //process mouse input
        void processMouse( float x, float y, bool lb, bool rb );
        //process kb input
        void processKb( std::string *skey, bool bDown );
        //returns true if has renderer
        bool hasRenderer( void );
        //set renderer
        void setRenderer( renderer_gui *g );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //add gui
        void addGui( gui *g );
        //set editable
        void setEditMode( bool b );
        //returns true if editable
        bool isEditable( void );
        //sets hoverable
        void setHoverMode( bool b );
        //returns true if hoverable
        bool isHoverable( void );
        //returns true if gui should swoosh in and out
        bool isFade( void );
        //sets fade mode
        void setFade( bool b );

        friend class gui;
    };
    
};

#endif