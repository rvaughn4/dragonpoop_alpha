
#include "dptaskpool_writelock.h"
#include "dptaskpool.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_writelock::dptaskpool_writelock( dptaskpool *p, dpmutex_writelock *l ) : shared_obj_writelock( p, l )
    {
        this->p = p;
    }

    //dtor
    dptaskpool_writelock::~dptaskpool_writelock( void )
    {

    }

    //return thread count
    unsigned int dptaskpool_writelock::getThreadCount( void )
    {
        return this->p->getThreadCount();
    }

    //add task, will create ref so caller retains original
    void dptaskpool_writelock::addTask( dptask *t )
    {
        this->p->addTask( t );
    }

    //add task, will create ref so caller retains original
    void dptaskpool_writelock::addTask( dptask_ref *t )
    {
        this->p->addTask( t );
    }

};
