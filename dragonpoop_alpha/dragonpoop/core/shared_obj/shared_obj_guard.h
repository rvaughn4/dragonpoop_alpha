
#ifndef dragonpoop_shared_obj_guard_h
#define dragonpoop_shared_obj_guard_h

#include <stdint.h>

namespace dragonpoop
{

    class shared_obj_interface;
    class shared_obj_readlock;
    class shared_obj_writelock;
    class shared_obj_lock;

    class shared_obj_guard
    {

    private:

        shared_obj_lock *l;

    protected:


    public:

        //ctor
        shared_obj_guard( void );
        //dtor
        virtual ~shared_obj_guard( void );
        //blocking read lock
        shared_obj_readlock *readLock( shared_obj_interface *o );
        //blocking write lock
        shared_obj_writelock *writeLock( shared_obj_interface *o );
        //read lock with timeout
        shared_obj_readlock *tryReadLock( shared_obj_interface *o, uint64_t ms );
        //write lock with timeout
        shared_obj_writelock *tryWriteLock( shared_obj_interface *o, uint64_t ms );
        //attempt read lock
        shared_obj_readlock *tryReadLock( shared_obj_interface *o );
        //attempt write lock
        shared_obj_writelock *tryWriteLock( shared_obj_interface *o );
        //unlock
        void unlock( void );
        
    };
    
};

#endif