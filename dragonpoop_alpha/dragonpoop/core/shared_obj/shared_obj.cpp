
#include "shared_obj.h"
#include "../dpmutex/dpmutexes.h"
#include "shared_obj_readlock.h"
#include "shared_obj_writelock.h"
#include "shared_obj_ref.h"
#include "shared_obj_guard.h"

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
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "shared_obj::~shared_obj" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "shared_obj::~shared_obj" );
        o.unlock();
        this->mm->destroyMutex( &this->l );
    }

    //blocking read lock
    shared_obj_readlock *shared_obj::readLock( const char *c_lock_location )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->readLock( c_lock_location );
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //blocking write lock
    shared_obj_writelock *shared_obj::writeLock( const char *c_lock_location )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->writeLock( c_lock_location );
        if( !l )
            return 0;
        r = this->genWriteLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //read lock with timeout
    shared_obj_readlock *shared_obj::tryReadLock( const char *c_lock_location, uint64_t ms )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->tryReadLock( c_lock_location, ms );
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //write lock with timeout
    shared_obj_writelock *shared_obj::tryWriteLock( const char *c_lock_location, uint64_t ms )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->tryWriteLock( c_lock_location, ms );
        if( !l )
            return 0;
        r = this->genWriteLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //attempt read lock
    shared_obj_readlock *shared_obj::tryReadLock( const char *c_lock_location )
    {
        shared_obj_readlock *r;
        dpmutex_readlock *l = this->l->tryReadLock( c_lock_location );
        if( !l )
            return 0;
        r = this->genReadLock( this, l );
        if( !r )
            dpmutex_lock::unlock( &l );
        return r;
    }

    //attempt write lock
    shared_obj_writelock *shared_obj::tryWriteLock( const char *c_lock_location )
    {
        shared_obj_writelock *r;
        dpmutex_writelock *l = this->l->tryWriteLock( c_lock_location );
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

    //generate read lock
    shared_obj_readlock *shared_obj::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new shared_obj_readlock( p, l );
    }
    
    //generate write lock
    shared_obj_writelock *shared_obj::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new shared_obj_writelock( p, l );
    }
    
    //generate ref
    shared_obj_ref *shared_obj::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new shared_obj_ref( p, k );
    }
    
};
