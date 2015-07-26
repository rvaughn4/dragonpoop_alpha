
#ifndef dragonpoop_dptask_writelock_h
#define dragonpoop_dptask_writelock_h

#include "../shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{

    class dptask_owner;
    class dptask_writelock;
    class dpthread_lock;
    class dptask;

    class dptask_writelock : public shared_obj_writelock
    {

    private:

        dptask *t;

    protected:

        //ctor
        dptask_writelock( dptask *t, dpmutex_writelock *l );
        //dtor
        virtual ~dptask_writelock( void );

    public:

        //run task
        void run( dptask_writelock *wl, dpthread_lock *thd );
        //returns true if task is alive
        bool isAlive( void );
        //kill task
        void kill( void );
        //get task sleep delay
        uint64_t getDelay( void );
        //set task sleep delay
        void setDelay( uint64_t ms );
        //returns true if task should run on one and the same thread only
        bool isSingleThread( void );
        //set one thread only mode
        void setSingleThread( bool b );
        //returns last ran time
        uint64_t getLastTime( void );
        //set last ran time
        void setLastTime( uint64_t t );

        friend class dptask;
    };
    
};

#endif