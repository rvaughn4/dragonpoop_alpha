
#include "dpmutex_readlock.h"

namespace dragonpoop
{

    //ctor
    dpmutex_readlock::dpmutex_readlock( dpmutex *m ) : dpmutex_lock( m )
    {

    }

    //dtor
    dpmutex_readlock::~dpmutex_readlock( void )
    {

    }

    //returns true if read lock
    bool dpmutex_readlock::isReadLock( void )
    {
        return 1;
    }

};
