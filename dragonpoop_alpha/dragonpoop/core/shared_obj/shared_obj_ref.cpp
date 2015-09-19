
#include "shared_obj_ref.h"

namespace dragonpoop
{
    
    //ctor
    shared_obj_ref::shared_obj_ref( shared_obj_interface *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        this->p = p;
        this->k = *k;
    }

    //dtor
    shared_obj_ref::~shared_obj_ref( void )
    {
        this->unlink();
    }

    //return a new ref
    shared_obj_ref *shared_obj_ref::getRef( void )
    {
        return 0;
    }

    //returns true if linked
    bool shared_obj_ref::isLinked( void )
    {
        if( !this->k.get() )
            return 0;
        return this->k->isLinked();
    }

    //unlink ref
    void shared_obj_ref::unlink( void )
    {
        if( !this->k.get() )
            return;
        this->k->lock();
        this->k->unlinkRef();
        this->k->unlock();
        this->k.reset();
    }

    //blocking read lock
    shared_obj_readlock *shared_obj_ref::readLock( const char *c_lock_location )
    {
        shared_obj_readlock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->readLock( c_lock_location );
        this->k->unlock();
        return r;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj_ref::writeLock( const char *c_lock_location )
    {
        shared_obj_writelock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->writeLock( c_lock_location );
        this->k->unlock();
        return r;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj_ref::tryReadLock( const char *c_lock_location, uint64_t ms )
    {
        shared_obj_readlock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->tryReadLock( c_lock_location, ms );
        this->k->unlock();
        return r;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj_ref::tryWriteLock( const char *c_lock_location, uint64_t ms )
    {
        shared_obj_writelock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->tryWriteLock( c_lock_location, ms );
        this->k->unlock();
        return r;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj_ref::tryReadLock( const char *c_lock_location )
    {
        shared_obj_readlock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->tryReadLock( c_lock_location );
        this->k->unlock();
        return r;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj_ref::tryWriteLock( const char *c_lock_location )
    {
        shared_obj_writelock *r;

        if( !this->k.get() )
            return 0;
        this->k->lock();

        if( !this->isLinked() )
        {
            this->k->unlock();
            return 0;
        }

        r = this->p->tryWriteLock( c_lock_location );
        this->k->unlock();
        return r;
    }

};
