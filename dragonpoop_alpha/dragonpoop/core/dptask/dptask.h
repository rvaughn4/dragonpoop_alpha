
#ifndef dragonpoop_dptask_h
#define dragonpoop_dptask_h

#include "../shared_obj/shared_obj.h"
#include <string>

namespace dragonpoop
{

    class dptask_owner;
    class dptask_writelock;
    class dpthread_lock;

    class dptask : public shared_obj
    {

    private:

        std::atomic<bool> bIsRun, bIsSingle, bShare;
        std::atomic<uint64_t> delayms, lasttime, last_tally_time, last_ran_time, ms_ran, ms_delay;
        unsigned int percent_usage;
        dptask_owner *o;
        std::string sname;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
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
        //get name
        void getName( std::string *s );
        //returns percent usage
        unsigned int getUsage( void );
        //returns true if task can share thread with static task
        bool canShare( void );

    public:

        //ctor
        dptask( dpmutex_master *mm, dptask_owner *o, uint64_t ms_delay, bool isSingleThread, bool bShare, const char *cname );
        //dtor
        virtual ~dptask( void );

        friend class dptask_readlock;
        friend class dptask_writelock;
    };

};

#endif
