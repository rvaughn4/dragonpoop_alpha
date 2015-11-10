
#ifndef dragonpoop_dpsky_man_writelock_h
#define dragonpoop_dpsky_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

#include <list>

namespace dragonpoop
{
    
    class dpsky_man;
    class core;
    class dpthread_lock;
    
    class dpsky_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        dpsky_man *t;
        
    protected:
        
        //ctor
        dpsky_man_writelock( dpsky_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpsky_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
        void run( dpthread_lock *thd );
        
        friend class dpsky_man;
    };
    
};

#endif