
#ifndef dragonpoop_renderer_gui_h
#define dragonpoop_renderer_gui_h

#include "../../gfx/gui.h"

namespace dragonpoop
{
    
    class renderer_gui_writelock;
    class gui_ref;
    class gui_writelock;
    
    class renderer_gui : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id, pid;
        bool bHasFg, bHasBg;
        gui_dims pos;
        gui_ref *g;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run gui
        void run( dpthread_lock *thd, renderer_gui_writelock *g );
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
        
    public:
        
        //ctor
        renderer_gui( renderer_writelock *r, gui_writelock *g );
        //dtor
        virtual ~renderer_gui( void );
        //return core
        core *getCore( void );
        //compares parent id
        bool compareParentId( dpid id );
        
        friend class renderer_gui_readlock;
        friend class renderer_gui_writelock;
        
    };
    
};

#endif