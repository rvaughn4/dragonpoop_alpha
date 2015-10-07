
#ifndef dragonpoop_gui_factory_h
#define dragonpoop_gui_factory_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class gui;
    class gfx_ref;
    class core;
    class dpthread_lock;
    class gfx_writelock;
    
    class gui_factory : public shared_obj
    {
        
    private:
        
        gfx_ref *g;

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //make gui
        gui *makeGui( dpthread_lock *thd, gui *p );
        //generate gui
        virtual gui *genGui( gfx_writelock *g, dpid id, dpid pid );
        
    public:
        
        //ctor
        gui_factory( core *c );
        //dtor
        virtual ~gui_factory( void );
        
        friend class gui_factory_readlock;
        friend class gui_factory_writelock;
        
    };
    
};

#endif