
#include "shared_obj_lock.h"
#include "../dpmutex/dpmutex_lock.h"

namespace dragonpoop
{

    //ctor
    shared_obj_lock::shared_obj_lock( shared_obj_interface *p, dpmutex_lock *l )
    {
        this->p = p;
        this->l = l;
    }

    //dtor
    shared_obj_lock::~shared_obj_lock( void )
    {
        dpmutex_lock::unlock( &this->l );
    }

    //returns true if linked
    bool shared_obj_lock::isLinked( void )
    {
        return 0;
    }

    //blocking read lock
    shared_obj_readlock *shared_obj_lock::readLock( const char *c_lock_location )
    {
        return 0;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj_lock::writeLock( const char *c_lock_location )
    {
        return 0;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj_lock::tryReadLock( const char *c_lock_location, uint64_t ms )
    {
        return 0;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj_lock::tryWriteLock( const char *c_lock_location, uint64_t ms )
    {
        return 0;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj_lock::tryReadLock( const char *c_lock_location )
    {
        return 0;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj_lock::tryWriteLock( const char *c_lock_location )
    {
        return 0;
    }

    //return a new ref
    shared_obj_ref *shared_obj_lock::getRef( void )
    {
        return this->p->getRef();
    }

    //unlink ref
    void shared_obj_lock::unlink( void )
    {
        this->p->unlink();
    }

};
