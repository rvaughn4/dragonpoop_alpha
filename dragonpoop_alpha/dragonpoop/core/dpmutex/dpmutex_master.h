
#ifndef dragonpoop_dpmutex_master_h
#define dragonpoop_dpmutex_master_h

#include <stdint.h>
#include "dpmutex.h"

namespace dragonpoop
{

    class dpspinlock;
    class dpmutex;
    class dpmutex_lock;
    class dpmutex_readlock;
    class dpmutex_writelock;

    class dpmutex_master
    {

    private:

        dpspinlock *slk;

    protected:

    public:

        //ctor
        dpmutex_master( void );
        //dtor
        virtual ~dpmutex_master( void );
        //generate new mutex
        dpmutex *genMutex( void );
        //destroy mutex
        void destroyMutex( dpmutex **m );
        //attempt to generate read lock on mutex
        dpmutex_readlock *genReadLock( dpmutex *m, uint64_t wait_ms );
        //atempt to generate write lock on mutex
        dpmutex_writelock *genWriteLock( dpmutex *m, uint64_t wait_ms );
        //destroy lock
        void destroyLock( dpmutex_lock **l );
        //write lock with wait time/timeout in ms
        dpmutex_writelock *writeLock( dpmutex *m, uint64_t t );
        //read lock with timeout in ms
        dpmutex_readlock *readLock( dpmutex *m, uint64_t t );
        //does not return until write locked
        dpmutex_writelock *writeLock( dpmutex *m );
        //does not return until read locked
        dpmutex_readlock *readLock( dpmutex *m );

    };
    
};

#endif