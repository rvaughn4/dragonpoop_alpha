
#ifndef dragonpoop_dptaskpool_writelock_h
#define dragonpoop_dptaskpool_writelock_h

#include "../shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{

    class dptaskpool;
    class dptask_ref;
    class dptask;
    class dpthread_lock;

    class dptaskpool_writelock : public shared_obj_writelock
    {

    private:

        dptaskpool *p;

    protected:

        //ctor
        dptaskpool_writelock( dptaskpool *p, dpmutex_writelock *l );
        //dtor
        virtual ~dptaskpool_writelock( void );

    public:

        //return thread count
        unsigned int getThreadCount( void );
        //add task, will create ref so caller retains original
        void addTask( dptask *t );
        //add task, will create ref so caller retains original
        void addTask( dptask_ref *t );
        //generate id
        dpid genId( void );
        //lock a thread from pool
        dpthread_lock *lockThread( void );

        friend class dptaskpool;
        friend class dpthread;
    };
    
};

#endif