
#ifndef dragonpoop_gui_h
#define dragonpoop_gui_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include "../dpbitmap/dpbitmap.h"
#include <atomic>

namespace dragonpoop
{
    
    class dptaskpool_writelock;
    class dptask;
    class dpthread_lock;
    class core;
    class gui_writelock;
    class gfx_writelock;
    class gfx_ref;
    
    struct gui_dims
    {
        float x, y, w, h;
    };
    
    class gui : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id, pid;
        dpbitmap fgtex, bgtex;
        bool bHasFg, bHasBg, bRepaintFg, bRepaintBg;
        std::atomic<bool> bPosChanged, bTexChanged, bRedraw;
        gui_dims pos;
        gfx_ref *g;

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run gui
        void run( dpthread_lock *thd, gui_writelock *g );
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
        //set parent id
        void setParentId( dpid id );
        //get parent id
        dpid getParentId( void );
        
    public:
        
        //ctor
        gui( gfx_writelock *g, dpid id, dpid parent_id );
        //dtor
        virtual ~gui( void );
        //return core
        core *getCore( void );
        
        friend class gui_readlock;
        friend class gui_writelock;
        friend class gfx;
        
    };
    
};

#endif