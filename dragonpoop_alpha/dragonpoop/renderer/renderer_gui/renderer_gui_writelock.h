
#ifndef dragonpoop_renderer_gui_writelock_h
#define dragonpoop_renderer_gui_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    class renderer_gui;
    class core;
    struct gui_dims;
    class dpthread_lock;
    class gui_writelock;
    class dpmatrix;
    class renderer_writelock;
    
    class renderer_gui_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_gui *t;
        
    protected:
        
        //ctor
        renderer_gui_writelock( renderer_gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_gui_writelock( void );
        
    public:
        
        //run gui
        void run( dpthread_lock *thd );
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
        //called to force pos update
        void syncPos( void );
        //called to force bg update
        void syncBg( void );
        //called to force fg update
        void syncFg( void );
        //redo matrix
        void redoMatrix( dpthread_lock *thd, renderer_writelock *r, dpmatrix *p_matrix );
        //process mouse input
        bool processMouse( float x, float y, bool lb, bool rb );
        //returns true if alive
        bool isAlive( void );
        //kills gui
        void kill( void );
        //returns true if has focus
        bool hasFocus( void );
        //gets gui id of focused child
        bool getFocusChild( renderer_writelock *r, dpid *fid );

        friend class renderer_gui;
    };
    
};

#endif