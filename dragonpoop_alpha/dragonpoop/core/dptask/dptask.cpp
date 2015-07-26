
#include "dptask.h"
#include "dptask_owner.h"
#include "dptask_readlock.h"
#include "dptask_writelock.h"
#include "dptask_ref.h"

namespace dragonpoop
{

    //ctor
    dptask::dptask( dpmutex_master *mm, dptask_owner *o, uint64_t ms_delay, bool isSingleThread ) : shared_obj( mm )
    {
        this->o = o;
        this->bIsRun = 1;
        this->bIsSingle = isSingleThread;
        this->delayms = ms_delay;
        this->lasttime = 0;
    }

    //dtor
    dptask::~dptask( void )
    {
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
        this->o->run( wl, thd );
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

};
