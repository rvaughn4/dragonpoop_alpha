
#ifndef dragonpoop_gui_h
#define dragonpoop_gui_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../dpbitmap/dpbitmap.h"
#include "../dpvertex/dprgba.h"

#include <atomic>
#include <string>
#include <vector>
#include <queue>

namespace dragonpoop
{

    class dptaskpool_writelock;
    class dptask;
    class dpthread_lock;
    class core;
    class gui_writelock;
    class gfx_writelock;
    class gfx_readlock;
    class gfx_ref;
    class gui_ref;
    class renderer_gui;
    class renderer_gui_ref;
    class gui_man_ref;

    struct gui_dims
    {
        float x, y, w, h;
        float border_w, border_tex_w;
        bool bDrag;
    };

    struct gui_txt_loc
    {
        float x, y, w, h;
        unsigned int front, line_no, char_no;
    };

    struct gui_kb_event
    {
        std::string sname;
        bool bDown;
    };

    struct gui_mouse_event
    {
        float x, y, sx, sy;
        bool lb, rb;
    };

    class gui : public shared_obj
    {

    private:

        core *c;
        dpid id, pid;
        dpbitmap fgtex, bgtex;
        bool bHasFg, bHasBg, bRepaintFg, bRepaintBg, bWasBgDrawn, bWasFgDrawn;
        std::atomic<bool> bPosChanged, bBgChanged, bFgChanged, bRedraw;
        bool bOldLb, bOldRb, bShiftDown, cur_flash, bIsSel, bIsEdit, bIsHover, bWasSel, bIsFade;
        float mx, my;
        gui_dims pos;
        gfx_ref *g;
        gui_man_ref *mgr;
        renderer_gui_ref *r;
        unsigned int z, fnt_size, margin_size, top_margin;
        dprgba fnt_clr;
        std::string stxt;
        std::vector<gui_txt_loc> txt_loc;
        unsigned int last_txt_line, line_front, cursor, sel_cursor, sz_div, redraw_timer;
        uint64_t t_last_redraw, t_last_processing;
        std::queue<gui_kb_event *> kbe;
        std::queue<gui_mouse_event *> mse;

        //reset text loc
        void resetTxtLoc( void );
        //add text loc
        void addTxtLoc( float x, float y, float w, float h, unsigned int line_no, unsigned int char_no );
        //find cursor by mouse coords
        unsigned int findCursor( float x, float y );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run gui
        void run( dpthread_lock *thd, gui_writelock *g );
        //do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );
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
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //override to paint forground texture
        virtual void repaintFg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //override to customize font rendering
        virtual void drawText( dpbitmap *bm );
        //set parent id
        void setParentId( dpid id );
        //get parent id
        dpid getParentId( void );
        //returns true if has renderer
        bool hasRenderer( void );
        //set renderer
        void setRenderer( renderer_gui *g );
        //returns pointer to bg texture
        dpbitmap *getBg( void );
        //returns pointer to fg texture
        dpbitmap *getFg( void );
        //returns z order
        unsigned int getZ( void );
        //sets focus
        void setFocus( void );
        //returns true if has focus
        bool hasFocus( void );
        //process mouse input
        void processMouse( float x, float y, float sx, float sy, bool lb, bool rb );
        //process kb input
        void processKb( std::string *skey, bool bDown );
        //override to handle mouse button
        virtual bool handleMouseClick( float x, float y, float sx, float sy, bool isRight, bool isDown );
        //override to handle mouse movement
        virtual bool handleMouseMove( float x, float y, float sx, float sy );
        //override to handle keyboard button
        virtual bool handleKbEvent( std::string *skey, bool isDown );
        //set text
        void setText( const char *c );
        //set text
        void setText( std::string *s );
        //get text
        void getText( std::string *s );
        //set font size
        void setFontSize( unsigned int s );
        //get font size
        unsigned int getFontSize( void );
        //set font color
        void setFontColor( dprgba *c );
        //get font color
        dprgba *getFontColor( void );
        //backspace text
        void backspace( void );
        //delete text
        void delete_( void );
        //insert text
        void insert( const char *c );
        //move cursor left
        void left( void );
        //move cursor right
        void right( void );
        //move cursor to end
        void end( void );
        //move cursor to home
        void home( void );
        //move cursor to top
        void top( void );
        //move cursor to bottom
        void bottom( void );
        //move cursor up
        void up( void );
        //move cursor down
        void down( void );
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
        //generates new id
        dpid genId( void );
        //set margin size
        void setMargin( unsigned int m );
        //get margin size
        unsigned int getMargin( void );
        //returns true if gui should swoosh in and out
        bool isFade( void );
        //sets fade mode
        void setFade( bool b );
        //set top amrgin
        void setTopMargin( unsigned int m );
        //get top margin
        unsigned int getTopMargin( void );
        //set draggable mode
        void setDraggable( bool b );

    public:

        //ctor
        gui( gfx_writelock *g, dpid id );
        //dtor
        virtual ~gui( void );
        //return core
        core *getCore( void );

        friend class gui_readlock;
        friend class gui_writelock;
        friend class gfx;
        friend class gui_factory;

    };

};

#endif
