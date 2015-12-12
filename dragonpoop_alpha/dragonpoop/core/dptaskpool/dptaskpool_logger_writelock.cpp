
#include "dptaskpool_logger_writelock.h"
#include "dptaskpool_logger.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_logger_writelock::dptaskpool_logger_writelock( dptaskpool_logger *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    dptaskpool_logger_writelock::~dptaskpool_logger_writelock( void )
    {

    }

    //get position time
    uint64_t dptaskpool_logger_writelock::getTime( void )
    {
        return this->t->getTime();
    }

    //get task list
    unsigned int dptaskpool_logger_writelock::getTaskList( dptaskpool_logger_value **p )
    {
        return this->t->getTaskList( p );
    }

    //add task
    void dptaskpool_logger_writelock::addTask( unsigned int tid, dptask *t )
    {
        this->t->addTask( tid, t );
    }

    //add task
    void dptaskpool_logger_writelock::addTask( unsigned int tid, dptask_ref *t )
    {
        this->t->addTask( tid, t );
    }

    //add task
    void dptaskpool_logger_writelock::addTask( unsigned int tid, dptask_readlock *t )
    {
        this->t->addTask( tid, t );
    }

    //clear
    void dptaskpool_logger_writelock::clear( void )
    {
        this->t->clear();
    }

    //sync
    void dptaskpool_logger_writelock::sync( dptaskpool_logger_ref *r )
    {
        this->t->sync( r );
    }

    //set time
    void dptaskpool_logger_writelock::setTime( uint64_t t )
    {
        this->t->setTime( t );
    }

};
