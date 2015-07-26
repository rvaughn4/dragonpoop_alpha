
#include "dptaskpool.h"
#include "dptaskpool_ref.h"
#include "dptaskpool_readlock.h"
#include "dptaskpool_writelock.h"
#include "../dptask/dptasks.h"
#include "../dpthread/dpthreads.h"
#include "../shared_obj/shared_obj_guard.h"
#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{

    //ctor
    dptaskpool::dptaskpool( dpmutex_master *mm, unsigned int thread_cnt ) : shared_obj( mm )
    {
        this->mm = mm;
        memset( &this->tasks, 0, sizeof( this->tasks ) );
        memset( &this->threads, 0, sizeof( this->threads ) );
        this->setThreadCount( thread_cnt );
    }

    //dtor
    dptaskpool::~dptaskpool( void )
    {
        dptask_ref *r;
        dpthread *t;
        dptaskpool_writelock *wl;

        wl = (dptaskpool_writelock *)this->writeLock();
        delete wl;

        t = this->popThread();
        while( t )
        {
            t->kill();
            delete t;
            t = this->popThread();
        }

        wl = (dptaskpool_writelock *)this->writeLock();
        if( this->threads.buffer )
            free( this->threads.buffer );

        r = this->popTask();
        while( r )
        {
            delete r;
            r = this->popTask();
        }
        if( this->tasks.buffer )
            free( this->tasks.buffer );

        this->unlink();

        delete wl;
    }

    //generate read lock
    shared_obj_readlock *dptaskpool::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dptaskpool_readlock( (dptaskpool *)p, l );
    }

    //generate write lock
    shared_obj_writelock *dptaskpool::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dptaskpool_writelock( (dptaskpool *)p, l );
    }

    //generate ref
    shared_obj_ref *dptaskpool::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dptaskpool_ref( (dptaskpool *)p, k );
    }

    //add task, will create ref so caller retains original
    void dptaskpool::addTask( dptask *t )
    {
        dptask_ref *at;
        dptask_writelock *tl;
        shared_obj_guard g;

        tl = (dptask_writelock *)g.writeLock( t );
        if( !tl )
            return;
        at = (dptask_ref *)tl->getRef();
        this->pushTask( at );
    }

    //add task, will create ref so caller retains original
    void dptaskpool::addTask( dptask_ref *t )
    {
        dptask_ref *at;
        dptask_writelock *tl;
        shared_obj_guard g;

        tl = (dptask_writelock *)g.writeLock( t );
        if( !tl )
            return;
        at = (dptask_ref *)tl->getRef();
        this->pushTask( at );
    }

    //adjust thread count
    void dptaskpool::setThreadCount( unsigned int c )
    {
        unsigned int j;
        dpthread *t;

        j = this->getThreadCount();

        if( j < c )
        {
            while( j < c )
            {
                t = new dpthread( this->mm, j + 1, (dptaskpool_ref *)this->getRef() );
                this->pushThread( t );
                j++;
            }

            return;
        }

        while( j > c )
        {
            t = this->popThread();
            delete t;
            j--;
        }
    }

    //return thread count
    unsigned int dptaskpool::getThreadCount( void )
    {
        return this->threads.acnt;
    }

    //push task into pool
    void dptaskpool::pushTask( dptask_ref *t )
    {
        unsigned int nc, nm, i;
        dptask_ref **nb;

        if( this->tasks.buffer )
        {
            for( i = 0; i < this->tasks.cnt; i++ )
            {
                if( this->tasks.buffer[ i ] )
                    continue;
                this->tasks.buffer[ i ] = t;
                return;
            }
        }

        nc = this->tasks.cnt + 1;
        if( !this->tasks.buffer || nc >= this->tasks.max )
        {
            nm = nc * 2 + 3;
            nb = (dptask_ref **)malloc( nm * sizeof(dptask_ref *) );
            memset( nb, 0, nm * sizeof(dptask_ref *) );
            if( this->tasks.buffer )
            {
                memcpy( nb, this->tasks.buffer, this->tasks.max * sizeof(dptask_ref *) );
                free( this->tasks.buffer );
            }
            this->tasks.buffer = nb;
            this->tasks.max = nm;
        }

        this->tasks.buffer[ nc - 1 ] = t;
        this->tasks.cnt = nc;
    }

    //pop task from pool
    dptask_ref *dptaskpool::popTask( void )
    {
        unsigned int i;
        dptask_ref *r;

        if( !this->tasks.buffer )
            return 0;

        for( i = 0; i < this->tasks.cnt; i++ )
        {
            r = this->tasks.buffer[ i ];
            if( !r )
                continue;
            this->tasks.buffer[ i ] = 0;
            return r;
        }

        return 0;
    }

    //push thread into pool
    void dptaskpool::pushThread( dpthread *t )
    {
        unsigned int nc, nm, i;
        dpthread **nb;

        if( this->threads.buffer )
        {
            for( i = 0; i < this->threads.cnt; i++ )
            {
                if( this->threads.buffer[ i ] )
                    continue;
                this->threads.buffer[ i ] = t;
                this->threads.acnt++;
                return;
            }
        }

        nc = this->threads.cnt + 1;
        if( !this->threads.buffer || nc >= this->threads.max )
        {
            nm = nc * 2 + 3;
            nb = (dpthread **)malloc( nm * sizeof(dpthread *) );
            memset( nb, 0, nm * sizeof(dpthread *) );
            if( this->threads.buffer )
            {
                memcpy( nb, this->threads.buffer, this->threads.max * sizeof(dpthread *) );
                free( this->threads.buffer );
            }
            this->threads.buffer = nb;
            this->threads.max = nm;
        }

        this->threads.buffer[ nc - 1 ] = t;
        this->threads.cnt = nc;
        this->threads.acnt++;
    }

    //pop thread from pool
    dpthread *dptaskpool::popThread( void )
    {
        unsigned int i;
        dpthread *r;

        if( !this->threads.buffer )
            return 0;

        for( i = 0; i < this->threads.cnt; i++ )
        {
            r = this->threads.buffer[ i ];
            if( !r )
                continue;
            this->threads.buffer[ i ] = 0;
            this->threads.acnt--;
            return r;
        }

        return 0;
    }

};
