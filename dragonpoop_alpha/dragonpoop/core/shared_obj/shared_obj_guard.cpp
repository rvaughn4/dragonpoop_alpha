
#include "shared_obj_guard.h"
#include "shared_obj_interface.h"
#include "shared_obj_lock.h"
#include "shared_obj_readlock.h"
#include "shared_obj_writelock.h"

namespace dragonpoop
{

    //ctor
    shared_obj_guard::shared_obj_guard( void )
    {
        this->l = 0;
    }

    //dtor
    shared_obj_guard::~shared_obj_guard( void )
    {
        this->unlock();
    }

    //blocking read lock
    shared_obj_readlock *shared_obj_guard::readLock( shared_obj_interface *o )
    {
        this->unlock();
        this->l = o->readLock();
        return (shared_obj_readlock *)this->l;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj_guard::writeLock( shared_obj_interface *o )
    {
        this->unlock();
        this->l = o->writeLock();
        return (shared_obj_writelock *)this->l;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj_guard::tryReadLock( shared_obj_interface *o, uint64_t ms )
    {
        this->unlock();
        this->l = o->tryReadLock( ms );
        return (shared_obj_readlock *)this->l;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj_guard::tryWriteLock( shared_obj_interface *o, uint64_t ms )
    {
        this->unlock();
        this->l = o->tryWriteLock( ms );
        return (shared_obj_writelock *)this->l;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj_guard::tryReadLock( shared_obj_interface *o )
    {
        this->unlock();
        this->l = o->tryReadLock();
        return (shared_obj_readlock *)this->l;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj_guard::tryWriteLock( shared_obj_interface *o )
    {
        this->unlock();
        this->l = o->tryWriteLock();
        return (shared_obj_writelock *)this->l;
    }

    //unlock
    void shared_obj_guard::unlock( void )
    {
        if( !this->l )
            return;
        delete this->l;
        this->l = 0;
    }

};
