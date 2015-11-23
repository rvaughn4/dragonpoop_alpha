
#include "dpheight_cache_readlock.h"
#include "dpheight_cache.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache_readlock::dpheight_cache_readlock( dpheight_cache *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    dpheight_cache_readlock::~dpheight_cache_readlock( void )
    {

    }

    //get camera position
    dpposition *dpheight_cache_readlock::getPosition( void )
    {
        return this->t->getPosition();
    }

    //get position time
    uint64_t dpheight_cache_readlock::getTime( void )
    {
        return this->t->getTime();
    }

    //get height at coord
    float dpheight_cache_readlock::getHeight( double x, double z )
    {
        return this->t->getHeight( x, z );
    }

};
