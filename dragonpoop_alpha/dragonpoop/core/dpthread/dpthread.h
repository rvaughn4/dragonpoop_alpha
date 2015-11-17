
#ifndef dragonpoop_dpthread_h
#define dragonpoop_dpthread_h

#include "dpthread_interface.h"
#include <thread>
#include <atomic>

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

    #define dpthread_max_tasks 100

    class dpthread : public dpthread_interface
    {

    private:

        dpmutex *l;
        dpmutex_master *lm;
        struct
        {
            std::atomic<dptask_ref *> static_ran[dpthread_max_tasks], static_notran[dpthread_max_tasks], dynamic_ran[dpthread_max_tasks], dynamic_notran[dpthread_max_tasks];
        } tasks;
        uint64_t ticks, epoch;
        std::thread *thd;
        std::atomic<bool> trun;
        unsigned int id;
        uint32_t idctr;
        float usage;

        //add static task
        bool addStatic( dptask_ref *t );
        //add dynamic task
        bool addDynamic( dptask_ref *t );
        //remove static task
        dptask_ref *removeDynamic( void );
        //delete all tasks
        void deleteTasks( void );


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
        dpthread( dpmutex_master *ml, unsigned int id );
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
