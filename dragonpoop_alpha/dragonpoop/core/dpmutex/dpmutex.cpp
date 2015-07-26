
#include "dpmutex.h"
#include "dpmutex_master.h"
#include "../dpspinlock/dpsponlock.h"

namespace dragonpoop
{

    //ctor
    dpmutex::dpmutex( dpmutex_master *m )
    {
        this->m = m;
    }

    //dtor
    dpmutex::~dpmutex( void )
    {
    }

    //try read lock
    dpmutex_readlock *dpmutex::tryReadLock( void )
    {
        return this->m->genReadLock( this, 1 );
    }

    //try write lock
    dpmutex_writelock *dpmutex::tryWriteLock( void )
    {
        return this->m->genWriteLock( this, 1 );
    }

    //try read lock until timeout in ms
    dpmutex_readlock *dpmutex::tryReadLock( uint64_t w )
    {
        return this->m->readLock( this, w );
    }

    //try write lock until timeout in ms
    dpmutex_writelock *dpmutex::tryWriteLock( uint64_t w )
    {
        return this->m->writeLock( this, w );
    }

    //do not return until read locked
    dpmutex_readlock *dpmutex::readLock( void )
    {
        return this->m->readLock( this );
    }

    //do not return until write locked
    dpmutex_writelock *dpmutex::writeLock( void )
    {
        return this->m->writeLock( this );
    }

    //add thread id to readlock list
    void dpmutex::addReadThread( std::thread::id tid )
    {
        dpmutex_thread *t;
        t = this->findReadThread( tid );
        t->cnt++;
    }

    //add thread id to writelock list
    void dpmutex::addWriteThread( std::thread::id tid )
    {
        dpmutex_thread *t;
        t = this->findWriteThread( tid );
        t->cnt++;
    }

    //remove thread id from readlock list
    void dpmutex::removeReadThread( std::thread::id tid )
    {
        dpmutex_thread *t;
        t = this->findReadThread( tid );
        if( t->cnt )
            t->cnt--;
    }

    //remove thread id from writelock list
    void dpmutex::removeWriteThread( std::thread::id tid )
    {
        dpmutex_thread *t;
        t = this->findWriteThread( tid );
        if( t->cnt )
            t->cnt--;
    }

    //return count of threads holding readlock that are not this thread
    int dpmutex::countReadThreads( std::thread::id tid )
    {
        unsigned int i, c;
        dpmutex_thread *r;
        int rv;

        c = (unsigned int)this->rids.size();
        rv = 0;
        for( i = 0; i < c; i++ )
        {
            r = &this->rids[ i ];
            if( r->tid == tid )
                continue;
            rv += r->cnt;
        }

        return rv;
    }

    //return count of threads holding writelock that are not this thread
    int dpmutex::countWriteThreads( std::thread::id tid )
    {
        unsigned int i, c;
        dpmutex_thread *r;
        int rv;

        c = (unsigned int)this->wids.size();
        rv = 0;
        for( i = 0; i < c; i++ )
        {
            r = &this->wids[ i ];
            if( r->tid == tid )
                continue;
            rv += r->cnt;
        }

        return rv;
    }

    //find thread with id
    dpmutex_thread *dpmutex::findReadThread( std::thread::id tid )
    {
        unsigned int i, c;
        dpmutex_thread *r;
        dpmutex_thread d;

        c = (unsigned int)this->rids.size();
        for( i = 0; i < c; i++ )
        {
            r = &this->rids[ i ];
            if( r->tid == tid )
                return r;
        }

        d.tid = tid;
        d.cnt = 0;
        this->rids.push_back( d );
        r = &this->rids[ c ];

        return r;
    }

    //find thread with id
    dpmutex_thread *dpmutex::findWriteThread( std::thread::id tid )
    {
        unsigned int i, c;
        dpmutex_thread *r;
        dpmutex_thread d;

        c = (unsigned int)this->wids.size();
        for( i = 0; i < c; i++ )
        {
            r = &this->wids[ i ];
            if( r->tid == tid )
                return r;
        }

        d.tid = tid;
        d.cnt = 0;
        this->wids.push_back( d );
        r = &this->wids[ c ];

        return r;
    }

};
