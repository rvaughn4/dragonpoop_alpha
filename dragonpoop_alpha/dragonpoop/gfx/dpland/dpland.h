
#ifndef dragonpoop_dpland_h
#define dragonpoop_dpland_h
/*
#include "../../core/shared_obj/shared_obj.h"
#include "../dpposition/dpposition.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpland_ref;
    class dpland_readlock;
    class dpland_writelock;
    class model_ref;
    class model_instance_ref;
    class gfx_ref;
    
    class dpland : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dpposition pos;
        model_ref *m;
        model_instance_ref *mi;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, dpland_writelock *g );
        //get position
        void getPosition( dpposition *p );
        //set position
        void setPosition( dpposition *p );
        
    public:
        
        //ctor
        dpland( core *c );
        //dtor
        virtual ~dpland( void );
        //return core
        core *getCore( void );
        
        friend class dpland_readlock;
        friend class dpland_writelock;
        
    };
    
};
*/
#endif