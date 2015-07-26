
#include "dpmutex_lock.h"
#include "dpmutex.h"
#include "dpmutex_master.h"

namespace dragonpoop
{

    //ctor
    dpmutex_lock::dpmutex_lock( dpmutex *m )
    {
        this->m = m;
    }

    //dtor
    dpmutex_lock::~dpmutex_lock( void )
    {

    }

    //returns mutex parent
    dpmutex *dpmutex_lock::getMutex( void )
    {
        return this->m;
    }

    //returns true if read lock
    bool dpmutex_lock::isReadLock( void )
    {
        return 0;
    }

    //returns true if write lock
    bool dpmutex_lock::isWriteLock( void )
    {
        return 0;
    }

    //destroy/unlock mutex lock
    void dpmutex_lock::unlock( dpmutex_lock **l )
    {
        dpmutex_lock *ll;
        if( !l )
            return;
        ll = *l;
        if( ll )
            ll->m->m->destroyLock( l );
    }

    //destroy/unlock mutex lock
    void dpmutex_lock::unlock( dpmutex_readlock **l )
    {
        dpmutex_lock::unlock( (dpmutex_lock **)l );
    }

    //destroy/unlock mutex lock
    void dpmutex_lock::unlock( dpmutex_writelock **l )
    {
        dpmutex_lock::unlock( (dpmutex_lock **)l );
    }
};
