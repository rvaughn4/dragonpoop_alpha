
#ifndef dragonpoop_renderer_gui_h
#define dragonpoop_renderer_gui_h

#include "../../gfx/gui/gui.h"
#include "../../gfx/dpmatrix/dpmatrix.h"

#define GUI_HOVER_MAX 0.2f

namespace dragonpoop
{
    
    class renderer_gui_writelock;
    class renderer_gui_readlock;
    class renderer_writelock;
    class gui_ref;
    class gui_writelock;
    class gui_readlock;
    class dpmatrix;
    
    class renderer_gui : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id, pid;
        bool bHasFg, bHasBg, bIsAlive;
        gui_dims pos;
        gui_ref *g;
        std::atomic<bool> bSyncPos, bSyncBg, bSyncFg;
        dpmatrix mat, size_mat, undo_mat;
        unsigned int z;
        float hv, opacity, fade;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run gui
        void run( dpthread_lock *thd, renderer_gui_writelock *g );
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
        //override to handle bg texture update
        virtual void updateBg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm );
        //override to handle fg texture update
        virtual void updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm );
        //called to force pos update
        void syncPos( void );
        //called to force bg update
        void syncBg( void );
        //called to force fg update
        void syncFg( void );
        //render model
        void render( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world );
        //redo matrix
        void redoMatrix( dpthread_lock *thd, renderer_writelock *r, renderer_gui_writelock *m, dpmatrix *p_matrix );
        //process mouse input
        bool processMouse( float x, float y, bool lb, bool rb );
        //returns opacity
        float getOpacity( void );
        //returns true if alive
        bool isAlive( void );
        //kills gui
        void kill( void );
        //returns true if has focus
        bool hasFocus( void );
        //gets gui id of focused child
        bool getFocusChild( renderer_writelock *r, dpid *fid );
        
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