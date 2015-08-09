
#include "shared_obj.h"
#include "../dpmutex/dpmutexes.h"
#include "shared_obj_readlock.h"
#include "shared_obj_writelock.h"
#include "shared_obj_ref.h"

namespace dragonpoop
{

    //ctor
    shared_obj::shared_obj( dpmutex_master *mm )
    {
        this->mm = mm;
        this->l = mm->genMutex();
    }

    //dtor
    shared_obj::~shared_obj( void )
    {
        dpmutex_writelock *wl;

        wl = this->l->writeLock();
        this->unlink();
        dpmutex_lock::unlock( &wl );

        this->mm->destroyMutex( &this->l );
    }

    //blocking read lock
    shared_obj_readlock *shared_obj::readLock( void )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->readLock();
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj::writeLock( void )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->writeLock();
        if( !l )
            return 0;
        r = this->genWriteLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj::tryReadLock( uint64_t ms )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->tryReadLock( ms );
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj::tryWriteLock( uint64_t ms )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->tryWriteLock( ms );
        if( !l )
            return 0;
        r = this->genWriteLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj::tryReadLock( void )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->tryReadLock();
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj::tryWriteLock( void )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->tryWriteLock();
        if( !l )
            return 0;
        r = this->genWriteLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //return a new ref
    shared_obj_ref *shared_obj::getRef( void )
    {
        std::shared_ptr<shared_obj_refkernal> k;

        this->removeDeadRefs();
        this->makeRef( &k );
        this->refs.push_back( k );

        return this->genRef( this, &k );
    }

    //returns true if linked
    bool shared_obj::isLinked( void )
    {
        std::list<std::shared_ptr<shared_obj_refkernal>> *l;
        std::list<std::shared_ptr<shared_obj_refkernal>>::iterator i;
        std::shared_ptr<shared_obj_refkernal> p;
        bool r;

        l = &this->refs;
        r = 0;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->lock();
            r = p->isLinked();
            p->unlock();
            if( r )
                return 1;
        }

        return 0;
    }

    //unlink ref
    void shared_obj::unlink( void )
    {
        std::list<std::shared_ptr<shared_obj_refkernal>> *l;
        std::list<std::shared_ptr<shared_obj_refkernal>>::iterator i;
        std::shared_ptr<shared_obj_refkernal> p;

        l = &this->refs;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->lock();
            p->unlinkOriginal();
            p->unlock();
            p.reset();
        }
        l->clear();
    }

    //remove dead refs
    void shared_obj::removeDeadRefs( void )
    {
        std::list<std::shared_ptr<shared_obj_refkernal>> *l, d;
        std::list<std::shared_ptr<shared_obj_refkernal>>::iterator i;
        std::shared_ptr<shared_obj_refkernal> p;

        l = &this->refs;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->lock();
            if( !p.get() || !p->isLinked() )
                d.push_back( p );
            p->unlock();
        }

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->refs.remove( p );
        }
        l->clear();
    }

    //generate refkernal
    bool shared_obj::makeRef( std::shared_ptr<shared_obj_refkernal> *k )
    {
        std::shared_ptr<shared_obj_refkernal> lk = std::make_shared<shared_obj_refkernal>();
        *k = lk;
        return 1;
    }

};
