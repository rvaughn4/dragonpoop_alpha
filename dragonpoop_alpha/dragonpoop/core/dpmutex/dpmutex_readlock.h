
#ifndef dragonpoop_dpmutex_readlock_h
#define dragonpoop_dpmutex_readlock_h

#include "dpmutex_lock.h"

namespace dragonpoop
{

    class dpmutex;

    class dpmutex_readlock : public dpmutex_lock
    {

    private:

    protected:

        //ctor
        dpmutex_readlock( dpmutex *m );
        //dtor
        virtual ~dpmutex_readlock( void );

    public:

        //returns true if read lock
        virtual bool isReadLock( void );

        friend class dpmutex_master;
    };
    
};

#endif