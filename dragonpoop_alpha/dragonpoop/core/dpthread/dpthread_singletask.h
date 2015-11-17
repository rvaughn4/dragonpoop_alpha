
#ifndef dragonpoop_dpthread_singletask_h
#define dragonpoop_dpthread_singletask_h

#include "dpthread_interface.h"
#include <thread>
#include <atomic>

namespace dragonpoop
{

    class dpthread_lock;
    class dpmutex;
    class dpmutex_master;
    class dpthread_singletask;

    void dpthread_st_threadproc( dpthread_singletask *t );

    class dpthread_singletask : public dpthread_interface
    {

    private:

        dpmutex *l;
        dpmutex_master *lm;
        uint64_t ticks, epoch;
        std::thread *thd;
        unsigned int id;
        uint32_t idctr;
        dptask_ref *tsk;
        std::atomic<bool> trun, tisrun;

    protected:

        //add new task (creates a ref)
        virtual void addTask( dptask_ref *t );
        //add new task (creates a ref)
        virtual void addTask( dptask *t );
        //returns true if has static tasks
        virtual bool hasStaticTask( void );
        //returns usage
        virtual float getUsage( void );
        //removes a dynamic task
        virtual dptask_ref *getTask( void );
        //returns task count
        virtual unsigned int countTasks( void );

    public:

        //ctor
        dpthread_singletask( dpmutex_master *ml, unsigned int id );
        //dtor
        virtual ~dpthread_singletask( void );
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

        friend void dpthread_st_threadproc( dpthread_singletask *t );
    };


};

#endif
