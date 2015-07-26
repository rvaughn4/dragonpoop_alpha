
#ifndef dragonpoop_dpthread_h
#define dragonpoop_dpthread_h

#include "dpthread_interface.h"
#include <thread>

namespace dragonpoop
{

    class dpthread_lock;
    class dpmutex;
    class dpmutex_master;

    struct dpthread_tasklist
    {
        dptask_ref **buffer;
        unsigned int cnt, max;
    };

    class dpthread;

    void dpthread_threadproc( dpthread *t );

    class dpthread : public dpthread_interface
    {

    private:

        dptaskpool_ref *tp;
        uint64_t tp_new_cnt, tp_dump_cnt;
        dpmutex *l;
        dpmutex_master *lm;
        struct
        {
            dpthread_tasklist torun, beenran;
        } tasks;
        uint64_t ticks, epoch;
        std::thread *thd;
        std::atomic<bool> trun;
        unsigned int id;
        uint32_t idctr;

        //add task to run
        void pushToRun( dptask_ref *t );
        //get and remove task from to run
        dptask_ref *popToRun( void );
        //add task to beenran
        void pushBeenRan( dptask_ref *t );
        //get and remove task from beenran
        dptask_ref *popBeenRan( void );
        //delete all tasks (or throw them on pool)
        void deleteTasks( void );
        //get new task from pool
        void getTaskFromPool( void );
        //dump old task back to pool
        void dumpTaskToPool( void );

    protected:

        //add new task (creates a ref)
        virtual void addTask( dptask_ref *t );
        //add new task (creates a ref)
        virtual void addTask( dptask *t );
        //add task pool
        virtual void addPool( dptaskpool *tp );

    public:

        //ctor
        dpthread( dpmutex_master *ml, unsigned int id );
        //ctor
        dpthread( dpmutex_master *ml, unsigned int id, dptaskpool_ref *tp );
        //dtor
        virtual ~dpthread( void );
        //get tick count in ms
        virtual uint64_t getTicks( void );
        //get epoch time in seconds
        virtual uint64_t getEpoch( void );
        //lock
        virtual dpthread_lock *lock( void );
        //return id
        virtual unsigned int getId( void );
        //kill thread
        virtual void kill( void );
        //generate unique dpid
        virtual dpid genId( void );

        friend void dpthread_threadproc( dpthread *t );
    };


};

#endif