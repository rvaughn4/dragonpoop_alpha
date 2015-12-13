
#include "dptask.h"
#include "dptask_owner.h"
#include "dptask_readlock.h"
#include "dptask_writelock.h"
#include "dptask_ref.h"
#include "../shared_obj/shared_obj_guard.h"
#include "../dpthread/dpthread_lock.h"
#include <chrono>

namespace dragonpoop
{

    //ctor
    dptask::dptask( dpmutex_master *mm, dptask_owner *o, uint64_t ms_delay, bool isSingleThread, bool bShare, const char *cname ) : shared_obj( mm )
    {
        this->o = o;
        this->bIsRun = 1;
        this->bShare = bShare;
        this->bIsSingle = isSingleThread;
        this->delayms = ms_delay;
        this->lasttime = 0;
        this->sname.assign( cname );
        this->percent_usage = 0;
        this->last_tally_time = 0;
        this->last_ran_time = 0;
        this->ms_delay = 0;
        this->ms_ran = 0;
    }

    //dtor
    dptask::~dptask( void )
    {
        shared_obj_guard o;
        o.tryWriteLock( this, 5000, "dptask::~dptask" );
        o.unlock();
        this->unlink();
    }

    //generate read lock
    shared_obj_readlock *dptask::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dptask_readlock( (dptask *)p, l );
    }

    //generate write lock
    shared_obj_writelock *dptask::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dptask_writelock( (dptask *)p, l );
    }

    //generate ref
    shared_obj_ref *dptask::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dptask_ref( (dptask *)p, k );
    }

    //run task
    void dptask::run( dptask_writelock *wl, dpthread_lock *thd )
    {
        uint64_t t, tt;
        std::chrono::time_point<std::chrono::steady_clock> tp_now;
        std::chrono::steady_clock::duration d_s;

        tp_now = std::chrono::steady_clock::now();
        d_s = tp_now.time_since_epoch();
        t = d_s.count() * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

        this->ms_delay += t - this->last_ran_time;

        this->o->run( wl, thd );

        tp_now = std::chrono::steady_clock::now();
        d_s = tp_now.time_since_epoch();
        tt = d_s.count() * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
        this->ms_ran += tt - t;
        this->last_ran_time = tt;

        tt = t - this->last_tally_time;
        if( tt > 1000 )
        {
            this->last_tally_time = t;
            tt = this->ms_delay + this->ms_ran;
            if( !tt )
                tt = 1;
            t = this->ms_ran;
            t = t * 100 / tt;
            if( t > 100 )
                t = 100;
            this->percent_usage = t;
            this->ms_delay = 0;
            this->ms_ran = 0;
        }
    }

    //returns true if task is alive
    bool dptask::isAlive( void )
    {
        return this->bIsRun;
    }

    //kill task
    void dptask::kill( void )
    {
        this->bIsRun = 0;
    }

    //get task sleep delay
    uint64_t dptask::getDelay( void )
    {
        return this->delayms;
    }

    //set task sleep delay
    void dptask::setDelay( uint64_t ms )
    {
        this->delayms = ms;
    }

    //returns true if task should run on one and the same thread only
    bool dptask::isSingleThread( void )
    {
        return this->bIsSingle;
    }

    //set one thread only mode
    void dptask::setSingleThread( bool b )
    {
        this->bIsSingle = b;
    }

    //returns last ran time
    uint64_t dptask::getLastTime( void )
    {
        return this->lasttime;
    }

    //set last ran time
    void dptask::setLastTime( uint64_t t )
    {
        this->lasttime = t;
    }

    //get name
    void dptask::getName( std::string *s )
    {
        s->assign( this->sname );
    }

    //returns percent usage
    unsigned int dptask::getUsage( void )
    {
        return this->percent_usage;
    }

    //returns true if task can share thread with static task
    bool dptask::canShare( void )
    {
        return this->bShare;
    }

};
