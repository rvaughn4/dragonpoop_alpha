
#ifndef dragonpoop_renderer_gui_writelock_h
#define dragonpoop_renderer_gui_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{
    class renderer_gui;
    class core;
    struct gui_dims;
    class dpthread_lock;
    class gui_writelock;
    class dpmatrix;
    class renderer_writelock;
    class renderer_gui_man_writelock;
    class renderer_gui_man_readlock;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;

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
        //process mouse input
        bool processMouse( renderer_gui_man_writelock *r, float x, float y, bool lb, bool rb, dpid focus_id );
        //process kb input
        bool processKb(  renderer_gui_man_writelock *r, std::string *sname, bool bIsDown );
        //returns true if alive
        bool isAlive( void );
        //kills gui
        void kill( void );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //get hovering gui id
        dpid getHoverId( void );
        //get focus gui id
        dpid getFocusId( void );
        //redo matrix
        void redoMatrix( dpthread_lock *thd, renderer_gui_man_writelock *r, dpmatrix *p_matrix );
        //run gui from background task
        void run( dpthread_lock *thd, render_api_context_writelock *ctx, renderer_gui_man_writelock *mgr );
        //render model
        void render( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //returns true if has focus
        bool hasFocus( void );
        //gets gui id of focused child
        bool getFocusChild( renderer_gui_man_writelock *r, dpid *fid );
        //returns z index
        unsigned int getZ( void );

        friend class renderer_gui;
    };

};

#endif
