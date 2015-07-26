
#ifndef dragonpoop_shared_obj_writelock_h
#define dragonpoop_shared_obj_writelock_h

#include "shared_obj_lock.h"

namespace dragonpoop
{

    class dpmutex_writelock;

    class shared_obj_writelock : public shared_obj_lock
    {

    private:

        shared_obj_interface *p;
        dpmutex_writelock *l;

    protected:

        //ctor
        shared_obj_writelock( shared_obj_interface *p, dpmutex_writelock *l );
        //dtor
        virtual ~shared_obj_writelock( void );

    public:

        //return a new ref
        virtual shared_obj_ref *getRef( void );
        //unlink ref
        virtual void unlink( void );

    };
    
};

#endif