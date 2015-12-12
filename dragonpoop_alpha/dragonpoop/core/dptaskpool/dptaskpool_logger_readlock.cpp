
#include "dptaskpool_logger_readlock.h"
#include "dptaskpool_logger.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_logger_readlock::dptaskpool_logger_readlock( dptaskpool_logger *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    dptaskpool_logger_readlock::~dptaskpool_logger_readlock( void )
    {

    }

    //get position time
    uint64_t dptaskpool_logger_readlock::getTime( void )
    {
        return this->t->getTime();
    }

    //get task list
    unsigned int dptaskpool_logger_readlock::getTaskList( dptaskpool_logger_value **p )
    {
        return this->t->getTaskList( p );
    }

};
