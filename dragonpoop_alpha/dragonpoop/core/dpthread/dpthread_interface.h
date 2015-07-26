
#ifndef dragonpoop_dpthread_interface_h
#define dragonpoop_dpthread_interface_h

#include <stdint.h>
#include "../dpid/dpid.h"

namespace dragonpoop
{

    class dpthread_lock;
    class dptask_ref;
    class dptask;
    class dptaskpool_ref;
    class dptaskpool;

    class dpthread_interface
    {

    private:

    protected:

        //ctor
        dpthread_interface( void );
        //lock
        virtual dpthread_lock *lock( void ) = 0;
        //add new task (creates a ref)
        virtual void addTask( dptask_ref *t ) = 0;
        //add new task (creates a ref)
        virtual void addTask( dptask *t ) = 0;
        //add task pool
        virtual void addPool( dptaskpool *tp ) = 0;

    public:

        //dtor
        virtual ~dpthread_interface( void );
        //get tick count in ms
        virtual uint64_t getTicks( void ) = 0;
        //get epoch time in seconds
        virtual uint64_t getEpoch( void ) = 0;
        //return id
        virtual unsigned int getId( void ) = 0;
        //kill thread
        virtual void kill( void ) = 0;
        //generate unique dpid
        virtual dpid genId( void ) = 0;

        friend class dpthread_lock;
    };
    
};

#endif