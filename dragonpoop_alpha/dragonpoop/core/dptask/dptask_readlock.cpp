
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

    //get name
    void dptask_readlock::getName( std::string *s )
    {
        this->t->getName( s );
    }

    //returns percent usage
    unsigned int dptask_readlock::getUsage( void )
    {
        return this->t->getUsage();
    }

    //returns true if task can share thread with static task
    bool dptask_readlock::canShare( void )
    {
        return this->t->canShare();
    }

};
