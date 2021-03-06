
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

    //returns true if has static tasks
    bool dpthread_lock::hasStaticTask( void )
    {
        return this->t->hasStaticTask();
    }

    //returns true if has static tasks that cannot share
    bool dpthread_lock::cannotShare( void )
    {
        return this->t->cannotShare();
    }

    //returns usage
    float dpthread_lock::getUsage( void )
    {
        return this->t->getUsage();
    }

    //removes a dynamic task
    dptask_ref *dpthread_lock::getTask( void )
    {
        return this->t->getTask();
    }

    //returns task count
    unsigned int dpthread_lock::countTasks( void )
    {
        return this->t->countTasks();
    }

    //returns static task count
    unsigned int dpthread_lock::countStaticTasks( void )
    {
        return this->t->countStaticTasks();
    }

    //get dynamic task list
    unsigned int dpthread_lock::getDynamicTaskRanList( std::atomic<dptask_ref *> **l )
    {
        return this->t->getDynamicTaskRanList( l );
    }

    //get static task list
    unsigned int dpthread_lock::getStaticTaskRanList( std::atomic<dptask_ref *> **l )
    {
        return this->t->getStaticTaskRanList( l );
    }

    //get dynamic task list
    unsigned int dpthread_lock::getDynamicTaskNotRanList( std::atomic<dptask_ref *> **l )
    {
        return this->t->getDynamicTaskNotRanList( l );
    }

    //get static task list
    unsigned int dpthread_lock::getStaticTaskNotRanList( std::atomic<dptask_ref *> **l )
    {
        return this->t->getStaticTaskNotRanList( l );
    }
};
