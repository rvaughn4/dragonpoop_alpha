
#ifndef dragonpoop_shared_obj_interface_h
#define dragonpoop_shared_obj_interface_h

#include <stdint.h>

namespace dragonpoop
{

    class dpmutex_readlock;
    class dpmutex_writelock;
    class shared_obj_readlock;
    class shared_obj_writelock;
    class shared_obj_ref;

    class shared_obj_interface
    {

    private:

    protected:

        //ctor
        shared_obj_interface( void );
        //blocking read lock
        virtual shared_obj_readlock *readLock( void ) = 0;
        //blocking write lock
        virtual shared_obj_writelock *writeLock( void ) = 0;
        //read lock with timeout
        virtual shared_obj_readlock *tryReadLock( uint64_t ms ) = 0;
        //write lock with timeout
        virtual shared_obj_writelock *tryWriteLock( uint64_t ms ) = 0;
        //attempt read lock
        virtual shared_obj_readlock *tryReadLock( void ) = 0;
        //attempt write lock
        virtual shared_obj_writelock *tryWriteLock( void ) = 0;
        //return a new ref
        virtual shared_obj_ref *getRef( void ) = 0;
        //unlink ref
        virtual void unlink( void ) = 0;

    public:

        //dtor
        virtual ~shared_obj_interface( void );
        //returns true if linked
        virtual bool isLinked( void ) = 0;

        friend class shared_obj_lock;
        friend class shared_obj_ref;
        friend class shared_obj_guard;
    };
    
};

#endif