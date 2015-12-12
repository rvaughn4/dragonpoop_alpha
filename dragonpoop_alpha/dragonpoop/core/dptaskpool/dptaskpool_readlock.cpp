
#include "dptaskpool_readlock.h"
#include "dptaskpool.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_readlock::dptaskpool_readlock( dptaskpool *p, dpmutex_readlock *l ) : shared_obj_readlock( p, l )
    {
        this->p = p;
    }

    //dtor
    dptaskpool_readlock::~dptaskpool_readlock( void )
    {

    }

    //return thread count
    unsigned int dptaskpool_readlock::getThreadCount( void )
    {
        return this->p->getThreadCount();
    }

    //generate id
    dpid dptaskpool_readlock::genId( void )
    {
        return this->p->genId();
    }

    //lock a thread from pool
    dpthread_lock *dptaskpool_readlock::lockThread( void )
    {
        return this->p->lockThread();
    }

    //get logger
    dptaskpool_logger_ref *dptaskpool_readlock::getLogger( void )
    {
        return this->p->getLogger();
    }

};
