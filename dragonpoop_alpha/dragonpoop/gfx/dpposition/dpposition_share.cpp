
#include "dpposition_share.h"
#include "dpposition_share_ref.h"
#include "dpposition_share_readlock.h"
#include "dpposition_share_writelock.h"

namespace dragonpoop
{

    //ctor
    dpposition_share::dpposition_share( dpmutex_master *mm ) : shared_obj( mm )
    {
        std::shared_ptr<std::atomic<uint64_t>> tt( new std::atomic<uint64_t>() );
        this->t = tt;
    }

    //dtor
    dpposition_share::~dpposition_share( void )
    {
    }

    //generate write lock
    shared_obj_writelock *dpposition_share::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpposition_share_writelock( (dpposition_share *)p, l );
    }

    //generate read lock
    shared_obj_readlock *dpposition_share::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpposition_share_readlock( (dpposition_share *)p, l );
    }

    //generate ref
    shared_obj_ref *dpposition_share::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpposition_share_ref( (dpposition_share *)p, k, this->t );
    }

    //set camera position
    void dpposition_share::setPosition( dpposition *p )
    {
        dpposition_inner i;
        this->pos.copy( p );
        p->getData( &i );
        *( this->t.get() ) = i.end.t;
    }

    //get camera position
    dpposition *dpposition_share::getPosition( void )
    {
        return &this->pos;
    }

    //get position time
    uint64_t dpposition_share::getTime( void )
    {
        return *( this->t.get() );
    }


};
