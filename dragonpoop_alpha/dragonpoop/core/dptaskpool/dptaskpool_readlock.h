
#ifndef dragonpoop_dptaskpool_readlock_h
#define dragonpoop_dptaskpool_readlock_h

#include "../shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{

    class dptaskpool;

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

        friend class dptaskpool;
    };
    
};

#endif