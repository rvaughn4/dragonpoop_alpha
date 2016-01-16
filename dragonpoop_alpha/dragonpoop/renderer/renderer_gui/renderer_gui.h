
#ifndef dragonpoop_renderer_gui_h
#define dragonpoop_renderer_gui_h

#include "../../gfx/gui/gui.h"
#include "../../gfx/dpmatrix/dpmatrix.h"
#include "../../gfx/dpvertex/dpvertexindex_buffer.h"

#define GUI_HOVER_MAX 0.1f

namespace dragonpoop
{

    class renderer_gui_writelock;
    class renderer_gui_readlock;
    class renderer_writelock;
    class gui_ref;
    class gui_writelock;
    class gui_readlock;
    class dpmatrix;
    class renderer_gui_man_writelock;
    class renderer_gui_man_readlock;
    class render_api_context_writelock;
    class render_api_texture_ref;
    class render_api_indexbuffer_ref;
    class render_api_vertexbuffer_ref;
    class render_api_commandlist_writelock;

    class renderer_gui : public shared_obj
    {

    private:

        core *c;
        dpid id, pid, hover_id, focus_id;
        bool bHasFg, bHasBg, bIsAlive, bIsHover, bIsEdit, bIsFade, bIsDrag;
        gui_dims pos;
        std::atomic<gui_ref *> g;
        std::atomic<bool> bSyncPos, bSyncBg, bSyncFg, bSyncBgRen, bSyncFgRen, bSyncPosRen;
        dpmatrix mat, undo_mat;
        unsigned int z;
        float hv, opacity, fade, clickfade;
        dpvertexindex_buffer bg_vb, fg_vb;
        uint64_t t_last_tex_update;
        render_api_texture_ref *render_tex_bg, *render_tex_fg;
        render_api_indexbuffer_ref *render_ib_fg, *render_ib_bg;
        render_api_vertexbuffer_ref *render_vb_fg, *render_vb_bg;
        struct
        {
            float x, y, ox, oy;
        } drag_pos;
        std::list<renderer_gui *> children;
        uint64_t t_last_children_sync;

        //delete children
        void deleteChildren( void );
        //run children
        void runChildren( dpthread_lock *thd, render_api_context_writelock *ctx );
        //sync children
        void syncChildren( dpthread_lock *thd );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
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
        //called to force pos update
        void syncPos( void );
        //called to force bg update
        void syncBg( void );
        //called to force fg update
        void syncFg( void );
        //redo matrix
        void redoMatrix( dpthread_lock *thd, renderer_gui_man_writelock *r, dpmatrix *p_matrix );
        //process mouse input
        bool processMouse( renderer_gui_man_writelock *r, float x, float y, bool lb, bool rb, dpid focus_id );
        //process kb input
        bool processKb( renderer_gui_man_writelock *r, std::string *sname, bool bIsDown );
        //returns opacity
        float getOpacity( void );
        //returns true if alive
        bool isAlive( void );
        //kills gui
        void kill( void );
        //returns true if has focus
        bool hasFocus( void );
        //gets gui id of focused child
        bool getFocusChild( renderer_gui_man_writelock *r, dpid *fid );
        //return bg vb
        dpvertexindex_buffer *getBgBuffer( void );
        //return fg vb
        dpvertexindex_buffer *getFgBuffer( void );
        //gets selected text from gui (copy or cut)
        bool getSelectedText( std::string *s, bool bDoCut );
        //sets selected text in gui (paste)
        bool setSelectedText( std::string *s );
        //get hovering gui id
        dpid getHoverId( void );
        //get focus gui id
        dpid getFocusId( void );
        //run gui from background task
        void run( dpthread_lock *thd, render_api_context_writelock *ctx );
        //render model
        void render( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //override to handle bg texture update
        void updateBg( dpbitmap *bm, render_api_context_writelock *ctx );
        //override to handle fg texture update
        void updateFg( dpbitmap *bm, render_api_context_writelock *ctx );
        //override to handle vb update
        void updateVb( gui_dims *p, render_api_context_writelock *ctx );

    public:

        //ctor
        renderer_gui( gui_writelock *g );
        //dtor
        virtual ~renderer_gui( void );
        //return core
        core *getCore( void );
        //compares parent id
        bool compareParentId( dpid id );
        //returns id
        dpid getId( void );
        //returns z order
        unsigned int getZ( void );

        friend class renderer_gui_readlock;
        friend class renderer_gui_writelock;

    };

};

#endif
