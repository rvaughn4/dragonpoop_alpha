
#include "dpthread_lock.h"
#include "../dpmutex/dpmutex_writelock.h"

namespace dragonpoop
{

    //ctor
    dpthread_lock::dpthread_lock( dpthread_interface *t, dpmutex_writelock *l )
    {
        this->t = t;
        this->l = l;
    }

    //dtor
    dpthread_lock::~dpthread_lock( void )
    {
        dpmutex_lock::unlock( &this->l );
    }

    //lock
    dpthread_lock *dpthread_lock::lock( void )
    {
        return 0;
    }

    //get tick count in ms
    uint64_t dpthread_lock::getTicks( void )
    {
        return this->t->getTicks();
    }

    //get epoch time in seconds
    uint64_t dpthread_lock::getEpoch( void )
    {
        return this->t->getEpoch();
    }

    //add new task (creates a ref)
    void dpthread_lock::addTask( dptask_ref *t )
    {
        this->t->addTask( t );
    }

    //add new task (creates a ref)
    void dpthread_lock::addTask( dptask *t )
    {
        this->t->addTask( t );
    }
    //add task pool
    void dpthread_lock::addPool( dptaskpool *tp )
    {
        this->t->addPool( tp );
    }

    //return id
    unsigned int dpthread_lock::getId( void )
    {
        return this->t->getId();
    }

    //kill thread
    void dpthread_lock::kill( void )
    {
        this->t->kill();
    }

    //generate unique dpid
    dpid dpthread_lock::genId( void )
    {
        return this->t->genId();
    }

};
