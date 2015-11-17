
#ifndef dragonpoop_dptaskpool_h
#define dragonpoop_dptaskpool_h

#include "../shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include <atomic>

namespace dragonpoop
{

    class dptask_ref;
    class dpthread;
    class dptask;
    class dptaskpool_writelock;
    class dpthread_lock;
    class dptask_writelock;
    class dpthread_interface;
    class dptask_writelock;
    class dpthread_lock;
    class dptaskpool_task;

    #define dptaskpool_max_threads 16
    #define dptaskpool_max_tasks 100

    class dptaskpool : public shared_obj
    {

    private:

        //threads
        std::atomic<dpthread_interface *> threads[ dptaskpool_max_threads ];
        //tasks not yet added
        std::atomic<dptask_ref *> tasks[ dptaskpool_max_tasks ];
        //mutex master
        dpmutex_master *mm;
        //task
        dptask *tsk;
        dptaskpool_task *tptsk;


        //find thread with fewest tasks
        dpthread_interface *getThread_fewestTasks( void );
        //find thread with no static tasks
        dpthread_interface *getThread_noStatic( void );
        //find thread with highest usage
        dpthread_interface *getThread_highUsage( void );
        //find thread with lowest usage
        dpthread_interface *getThread_lowUsage( void );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //add task, will create ref so caller retains original
        void addTask( dptask *t );
        //add task, will create ref so caller retains original
        void addTask( dptask_ref *t );
        //adjust thread count
        void setThreadCount( unsigned int c );
        //return thread count
        unsigned int getThreadCount( void );
        //push task into pool
        void pushTask( dptask_ref *t );
        //pop task from pool
        dptask_ref *popTask( void );
        //push thread into pool
        void pushThread( dpthread_interface *t );
        //pop thread from pool
        dpthread_interface *popThread( void );
        //generate id
        dpid genId( void );
        //lock a thread from pool
        dpthread_lock *lockThread( void );
        //run taskpool
        void run( dptask_writelock *tl, dpthread_lock *th );

    public:

        //ctor
        dptaskpool( dpmutex_master *mm, unsigned int thread_cnt );
        //dtor
        virtual ~dptaskpool( void );


        friend class dptaskpool_readlock;
        friend class dptaskpool_writelock;
        friend class dpthread;
    };

};

#endif
