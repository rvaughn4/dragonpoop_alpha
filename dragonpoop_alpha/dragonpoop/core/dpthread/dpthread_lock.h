
#ifndef dragonpoop_dpthread_lock_h
#define dragonpoop_dpthread_lock_h

#include "dpthread_interface.h"

namespace dragonpoop
{

    class dpthread;
    class dpmutex_writelock;

    class dpthread_lock : public dpthread_interface
    {

    private:

        dpthread_interface *t;
        dpmutex_writelock *l;

    protected:

        //ctor
        dpthread_lock( dpthread_interface *t, dpmutex_writelock *l );
        //lock
        virtual dpthread_lock *lock( void );

    public:

        //dtor
        virtual ~dpthread_lock( void );
        //get tick count in ms
        virtual uint64_t getTicks( void );
        //get epoch time in seconds
        virtual uint64_t getEpoch( void );
        //add new task (creates a ref)
        virtual void addTask( dptask_ref *t );
        //add new task (creates a ref)
        virtual void addTask( dptask *t );
        //add task pool
        virtual void addPool( dptaskpool *tp );
        //return id
        virtual unsigned int getId( void );
        //kill thread
        virtual void kill( void );
        //generate unique dpid
        virtual dpid genId( void );

        friend class dpthread;
    };
    
};

#endif