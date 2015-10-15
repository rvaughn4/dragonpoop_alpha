
#ifndef dragonpoop_dpactor_man_writelock_h
#define dragonpoop_dpactor_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    
    class dpactor_man;
    class core;
    class dpthread_lock;
    class dpactor;
    class dpactor_ref;
    
    class dpactor_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        dpactor_man *t;
        
    protected:
        
        //ctor
        dpactor_man_writelock( dpactor_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpactor_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
        void run( dpthread_lock *thd );
        //add actor
        void addActor( dpactor *a );
        //add actor
        void addActor( dpactor_ref *a );
        //return actor count
        unsigned int getActorCount( void );

        friend class dpactor_man;
    };
    
};

#endif