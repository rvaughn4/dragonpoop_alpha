
#include "../dpspinlock/dpsponlock.h"
#include "dpmutex_master.h"
#include "dpmutex.h"
#include "dpmutex_lock.h"
#include "dpmutex_writelock.h"
#include "dpmutex_readlock.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <iostream>

namespace dragonpoop
{

    //ctor
    dpmutex_master::dpmutex_master( void )
    {
        this->slk = new dpspinlock();
    }

    //dtor
    dpmutex_master::~dpmutex_master( void )
    {
        delete this->slk;
    }

    //generate new mutex
    dpmutex *dpmutex_master::genMutex( void )
    {
        dpmutex *r;

        this->slk->lock();

        r = new dpmutex( this );

        this->slk->unlock();

        return r;
    }

    //destroy mutex
    void dpmutex_master::destroyMutex( dpmutex **mm )
    {
        dpmutex *m;
        std::thread::id i = std::this_thread::get_id();

        if( !mm )
            return;
        m = *mm;
        if( !m )
            return;
        *mm = 0;

        this->slk->lock();

        if( m->countReadThreads( i ) > 0 || m->countWriteThreads( i ) > 0 )
            std::cerr << "PROBLEM!!!! mutex still locked!\r\n\r\n";
        else
            delete m;

        this->slk->unlock();
    }

    //attempt to generate read lock on mutex
    dpmutex_readlock *dpmutex_master::genReadLock( dpmutex *m, uint64_t wait_ms )
    {
        dpmutex_readlock *r;
        std::thread::id i = std::this_thread::get_id();

        r = 0;
        this->slk->lock();

        if( m->countWriteThreads( i ) > 0 )
        {
            this->slk->unlock();
            return 0;
        }

        r = new dpmutex_readlock( m );
        m->addReadThread( i );

        this->slk->unlock();
        
        return r;
    }

    //atempt to generate write lock on mutex
    dpmutex_writelock *dpmutex_master::genWriteLock( dpmutex *m, uint64_t wait_ms )
    {
        dpmutex_writelock *r;
        std::thread::id i = std::this_thread::get_id();

        r = 0;
        this->slk->lock();

        if( m->countReadThreads( i ) > 0 || m->countWriteThreads( i ) > 0 )
        {
            this->slk->unlock();
            return 0;
        }

        r = new dpmutex_writelock( m );
        m->addWriteThread( i );

        this->slk->unlock();
        
        return r;
    }

    //destroy lock
    void dpmutex_master::destroyLock( dpmutex_lock **ll )
    {
        dpmutex *m;
        dpmutex_lock *l;
        std::thread::id i = std::this_thread::get_id();

        if( !ll )
            return;
        l = *ll;
        if( !l )
            return;
        *ll = 0;

        this->slk->lock();

        m = l->getMutex();
        if( l->isReadLock() )
            m->removeReadThread( i );
        if( l->isWriteLock() )
            m->removeWriteThread( i );
        delete l;

        this->slk->unlock();
    }

    //write lock with wait time/timeout in ms
    dpmutex_writelock *dpmutex_master::writeLock( dpmutex *m, uint64_t t )
    {
        dpmutex_writelock *l;

        l = this->genWriteLock( m, t );
        if( l )
            return l;
        while( t )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            t--;
            l = this->genWriteLock( m, t );
            if( l )
                return l;
        }

        return 0;
    }

    //read lock with timeout in ms
    dpmutex_readlock *dpmutex_master::readLock( dpmutex *m, uint64_t t )
    {
        dpmutex_readlock *l;

        l = this->genReadLock( m, t );
        if( l )
            return l;
        while( t )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            t--;
            l = this->genReadLock( m, t );
            if( l )
                return l;
        }

        return 0;
    }

    //does not return until write locked
    dpmutex_writelock *dpmutex_master::writeLock( dpmutex *m )
    {
        dpmutex_writelock *l;

        do
        {
            l = this->genWriteLock( m, 100 );
            if( l )
                return l;
        }
        while( !l );

        return 0;
    }

    //does not return until read locked
    dpmutex_readlock *dpmutex_master::readLock( dpmutex *m )
    {
        dpmutex_readlock *l;

        do
        {
            l = this->genReadLock( m, 100 );
            if( l )
                return l;
        }
        while( !l );

        return 0;
    }

};
