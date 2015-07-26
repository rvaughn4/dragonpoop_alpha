
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
    shared_obj_readlock *shared_obj_ref::readLock( void )
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

        r = this->p->readLock();
        this->k->unlock();
        return r;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj_ref::writeLock( void )
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

        r = this->p->writeLock();
        this->k->unlock();
        return r;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj_ref::tryReadLock( uint64_t ms )
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

        r = this->p->tryReadLock( ms );
        this->k->unlock();
        return r;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj_ref::tryWriteLock( uint64_t ms )
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

        r = this->p->tryWriteLock( ms );
        this->k->unlock();
        return r;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj_ref::tryReadLock( void )
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

        r = this->p->tryReadLock();
        this->k->unlock();
        return r;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj_ref::tryWriteLock( void )
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

        r = this->p->tryWriteLock();
        this->k->unlock();
        return r;
    }

};
