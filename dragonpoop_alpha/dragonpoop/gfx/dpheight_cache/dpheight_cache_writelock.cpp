
#include "dpheight_cache_writelock.h"
#include "dpheight_cache.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache_writelock::dpheight_cache_writelock( dpheight_cache *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    dpheight_cache_writelock::~dpheight_cache_writelock( void )
    {

    }

    //set camera position
    void dpheight_cache_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }

    //get camera position
    dpposition *dpheight_cache_writelock::getPosition( void )
    {
        return this->t->getPosition();
    }

    //get position time
    uint64_t dpheight_cache_writelock::getTime( void )
    {
        return this->t->getTime();
    }

};
