
#include "dptask_readlock.h"
#include "dptask.h"

namespace dragonpoop
{

    //ctor
    dptask_readlock::dptask_readlock( dptask *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    dptask_readlock::~dptask_readlock( void )
    {

    }

    //returns true if task is alive
    bool dptask_readlock::isAlive( void )
    {
        return this->t->isAlive();
    }

    //get task sleep delay
    uint64_t dptask_readlock::getDelay( void )
    {
        return this->t->getDelay();
    }

    //returns true if task should run on one and the same thread only
    bool dptask_readlock::isSingleThread( void )
    {
        return this->t->isSingleThread();
    }

    //returns last ran time
    uint64_t dptask_readlock::getLastTime( void )
    {
        return this->getLastTime();
    }

};
