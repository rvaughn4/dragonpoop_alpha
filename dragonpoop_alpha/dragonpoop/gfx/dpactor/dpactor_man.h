
#ifndef dragonpoop_dpactor_man_h
#define dragonpoop_dpactor_man_h

#include "../../core/shared_obj/shared_obj.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpactor_man_ref;
    class dpactor_man_readlock;
    class dpactor_man_writelock;
    class gfx_ref;
    class dptask;
    class dpactor_man_task;
    class dptaskpool_writelock;
    class gfx;
    class dpactor;
    class dpactor_ref;
    
    class dpactor_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        dptask *tsk;
        dpactor_man_task *gtsk;
        std::list<dpactor_ref *> actors;
        int actor_cnt;

        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //delete all actors
        void deleteActors( void );
        //run all actors
        void runActors( dpthread_lock *thd );

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, dpactor_man_writelock *g );
        //add actor
        void addActor( dpactor *a );
        //add actor
        void addActor( dpactor_ref *a );
        //return actor count
        unsigned int getActorCount( void );

    public:
        
        //ctor
        dpactor_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~dpactor_man( void );
        //return core
        core *getCore( void );
        
        friend class dpactor_man_readlock;
        friend class dpactor_man_writelock;
        
    };
    
};

#endif