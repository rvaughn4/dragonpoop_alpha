
#include "dpheight_cache.h"
#include "dpheight_cache_ref.h"
#include "dpheight_cache_readlock.h"
#include "dpheight_cache_writelock.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache::dpheight_cache( dpmutex_master *mm ) : shared_obj( mm )
    {
        *( this->t.get() ) = 0;
    }

    //dtor
    dpheight_cache::~dpheight_cache( void )
    {
    }

    //generate write lock
    shared_obj_writelock *dpheight_cache::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpheight_cache_writelock( (dpheight_cache *)p, l );
    }

    //generate read lock
    shared_obj_readlock *dpheight_cache::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpheight_cache_readlock( (dpheight_cache *)p, l );
    }

    //generate ref
    shared_obj_ref *dpheight_cache::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpheight_cache_ref( (dpheight_cache *)p, k, this->t );
    }

    //set camera position
    void dpheight_cache::setPosition( dpposition *p )
    {
        dpposition_inner i;
        this->pos.copy( p );
        p->getData( &i );
        *( this->t.get() ) = i.end.t;
    }

    //get camera position
    dpposition *dpheight_cache::getPosition( void )
    {
        return &this->pos;
    }

    //get position time
    uint64_t dpheight_cache::getTime( void )
    {
        return *( this->t.get() );
    }


};
