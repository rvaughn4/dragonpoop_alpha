
#ifndef dragonpoop_gui_h
#define dragonpoop_gui_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class dptaskpool_writelock;
    class dptask;
    class dpthread_lock;
    class core;
    class gui_writelock;
    
    class gui : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id;

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
        
    public:
        
        //ctor
        gui( core *c, dpid id );
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