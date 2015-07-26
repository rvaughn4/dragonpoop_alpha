
#include "dptask_writelock.h"
#include "dptask.h"

namespace dragonpoop
{

    //ctor
    dptask_writelock::dptask_writelock( dptask *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    dptask_writelock::~dptask_writelock( void )
    {

    }

    //run task
    void dptask_writelock::run( dptask_writelock *wl, dpthread_lock *thd )
    {
        this->t->run( wl, thd );
    }

    //returns true if task is alive
    bool dptask_writelock::isAlive( void )
    {
        return this->t->isAlive();
    }

    //kill task
    void dptask_writelock::kill( void )
    {
        this->t->kill();
    }

    //get task sleep delay
    uint64_t dptask_writelock::getDelay( void )
    {
        return this->t->getDelay();
    }

    //set task sleep delay
    void dptask_writelock::setDelay( uint64_t ms )
    {
        this->t->setDelay( ms );
    }

    //returns true if task should run on one and the same thread only
    bool dptask_writelock::isSingleThread( void )
    {
        return this->t->isSingleThread();
    }

    //set one thread only mode
    void dptask_writelock::setSingleThread( bool b )
    {
        this->t->setSingleThread( b );
    }

    //returns last ran time
    uint64_t dptask_writelock::getLastTime( void )
    {
        return this->t->getLastTime();
    }

    //set last ran time
    void dptask_writelock::setLastTime( uint64_t t )
    {
        this->t->setLastTime( t );
    }

};
