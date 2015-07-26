
#include "dpmutex_writelock.h"

namespace dragonpoop
{

    //ctor
    dpmutex_writelock::dpmutex_writelock( dpmutex *m ) : dpmutex_lock( m )
    {

    }

    //dtor
    dpmutex_writelock::~dpmutex_writelock( void )
    {

    }

    //returns true if write lock
    bool dpmutex_writelock::isWriteLock( void )
    {
        return 1;
    }

};
