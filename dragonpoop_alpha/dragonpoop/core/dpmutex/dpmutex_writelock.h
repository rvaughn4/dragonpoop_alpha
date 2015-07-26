
#ifndef dragonpoop_dpmutex_writelock_h
#define dragonpoop_dpmutex_writelock_h

#include "dpmutex_lock.h"

namespace dragonpoop
{

    class dpmutex;

    class dpmutex_writelock : public dpmutex_lock
    {

    private:

    protected:

        //ctor
        dpmutex_writelock( dpmutex *m );
        //dtor
        virtual ~dpmutex_writelock( void );

    public:

        //returns true if write lock
        virtual bool isWriteLock( void );
        
        friend class dpmutex_master;
    };
    
};

#endif