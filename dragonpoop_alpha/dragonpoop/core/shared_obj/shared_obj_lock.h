
#ifndef dragonpoop_shared_obj_lock_h
#define dragonpoop_shared_obj_lock_h

#include "shared_obj_interface.h"

namespace dragonpoop
{

    class dpmutex_lock;

    class shared_obj_lock : public shared_obj_interface
    {

    private:

        shared_obj_interface *p;
        dpmutex_lock *l;

    protected:

        //ctor
        shared_obj_lock( shared_obj_interface *p, dpmutex_lock *l );
        //blocking read lock
        virtual shared_obj_readlock *readLock( void );
        //blocking write lock
        virtual shared_obj_writelock *writeLock( void );
        //read lock with timeout
        virtual shared_obj_readlock *tryReadLock( uint64_t ms );
        //write lock with timeout
        virtual shared_obj_writelock *tryWriteLock( uint64_t ms );
        //attempt read lock
        virtual shared_obj_readlock *tryReadLock( void );
        //attempt write lock
        virtual shared_obj_writelock *tryWriteLock( void );
        //return a new ref
        virtual shared_obj_ref *getRef( void );
        //unlink ref
        virtual void unlink( void );
        //dtor
        virtual ~shared_obj_lock( void );

    public:

        //returns true if linked
        virtual bool isLinked( void );

        friend class shared_obj_guard;
    };
    
};

#endif