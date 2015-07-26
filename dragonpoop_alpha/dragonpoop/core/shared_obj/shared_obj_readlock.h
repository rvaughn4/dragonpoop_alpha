
#ifndef dragonpoop_shared_obj_readlock_h
#define dragonpoop_shared_obj_readlock_h

#include "shared_obj_lock.h"

namespace dragonpoop
{

    class dpmutex_readlock;

    class shared_obj_readlock : public shared_obj_lock
    {

    private:

        shared_obj_interface *p;
        dpmutex_readlock *l;

    protected:

        //ctor
        shared_obj_readlock( shared_obj_interface *p, dpmutex_readlock *l );
        //dtor
        virtual ~shared_obj_readlock( void );

    public:


    };
    
};

#endif