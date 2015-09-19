
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
        shared_obj_readlock *readLock( shared_obj_interface *o, const char *c_lock_location );
        //blocking write lock
        shared_obj_writelock *writeLock( shared_obj_interface *o, const char *c_lock_location );
        //read lock with timeout
        shared_obj_readlock *tryReadLock( shared_obj_interface *o, uint64_t ms, const char *c_lock_location );
        //write lock with timeout
        shared_obj_writelock *tryWriteLock( shared_obj_interface *o, uint64_t ms, const char *c_lock_location );
        //attempt read lock
        shared_obj_readlock *tryReadLock( shared_obj_interface *o, const char *c_lock_location );
        //attempt write lock
        shared_obj_writelock *tryWriteLock( shared_obj_interface *o, const char *c_lock_location );
        //unlock
        void unlock( void );
        
    };
    
};

#endif