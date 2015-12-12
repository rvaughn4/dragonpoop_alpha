
#ifndef dragonpoop_dptaskpool_readlock_h
#define dragonpoop_dptaskpool_readlock_h

#include "../shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{

    class dptaskpool;
    class dpthread_lock;
    class dptaskpool_logger_ref;

    class dptaskpool_readlock : public shared_obj_readlock
    {

    private:

        dptaskpool *p;

    protected:

        //ctor
        dptaskpool_readlock( dptaskpool *p, dpmutex_readlock *l );
        //dtor
        virtual ~dptaskpool_readlock( void );

    public:

        //return thread count
        unsigned int getThreadCount( void );
        //generate id
        dpid genId( void );
        //lock a thread from pool
        dpthread_lock *lockThread( void );
        //get logger
        dptaskpool_logger_ref *getLogger( void );

        friend class dptaskpool;
    };

};

#endif
