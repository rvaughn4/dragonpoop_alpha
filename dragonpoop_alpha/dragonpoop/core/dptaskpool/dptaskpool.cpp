
#include "dptaskpool.h"
#include "dptaskpool_ref.h"
#include "dptaskpool_readlock.h"
#include "dptaskpool_writelock.h"
#include "../dptask/dptasks.h"
#include "../dpthread/dpthreads.h"
#include "../shared_obj/shared_obj_guard.h"
#include <stdlib.h>
#include <string.h>
#include <thread>

namespace dragonpoop
{

    //ctor
    dptaskpool::dptaskpool( dpmutex_master *mm, unsigned int thread_cnt ) : shared_obj( mm )
    {
        int i;

        this->mm = mm;

        for( i = 0; i < dptaskpool_max_tasks; i++ )
            this->tasks[ i ] = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
            this->threads[ i ] = 0;

        if( !thread_cnt )
            thread_cnt = std::thread::hardware_concurrency();
        if( thread_cnt < 3 )
            thread_cnt = 3;

        this->setThreadCount( thread_cnt );
    }

    //dtor
    dptaskpool::~dptaskpool( void )
    {
        dptask_ref *r;
        dpthread_interface *t;
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "dptaskpool::~dptaskpool" );
        o.unlock();
        this->unlink();

        t = this->popThread();
        while( t )
        {
            t->kill();
            delete t;
            t = this->popThread();
        }

        r = this->popTask();
        while( r )
        {
            delete r;
            r = this->popTask();
        }

        o.tryWriteLock( this, 5000, "dptaskpool::~dptaskpool" );
        o.unlock();
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

        tl = (dptask_writelock *)g.writeLock( t, "dptaskpool::addTask" );
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

        tl = (dptask_writelock *)g.writeLock( t, "dptaskpool::addTask" );
        if( !tl )
            return;
        at = (dptask_ref *)tl->getRef();
        this->pushTask( at );
    }

    //adjust thread count
    void dptaskpool::setThreadCount( unsigned int c )
    {
        unsigned int j;
        dpthread_interface *t;

        j = this->getThreadCount();

        if( j < c )
        {
            while( j < c )
            {
                t = new dpthread( this->mm, j + 1 );
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
        int i, r;

        r = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] != 0 )
                r++;
        }

        return r;
    }

    //push task into pool
    void dptaskpool::pushTask( dptask_ref *t )
    {
        int i;

        for( i = 0; i < dptaskpool_max_tasks; i++ )
        {
            if( this->tasks[ i ] != 0 )
                continue;
            this->tasks[ i ] = t;
            return;
        }

        delete t;
    }

    //pop task from pool
    dptask_ref *dptaskpool::popTask( void )
    {
        int i;
        dptask_ref *r;

        for( i = 0; i < dptaskpool_max_tasks; i++ )
        {
            if( this->tasks[ i ] == 0 )
                continue;
            r = this->tasks[ i ];
            this->tasks[ i ] = 0;
            return r;
        }

        return 0;
    }

    //push thread into pool
    void dptaskpool::pushThread( dpthread_interface *t )
    {
        int i;

        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] != 0 )
                continue;
            this->threads[ i ] = t;
            return;
        }

        delete t;
    }

    //pop thread from pool
    dpthread_interface *dptaskpool::popThread( void )
    {
        int i;
        dpthread_interface *r;

        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] == 0 )
                continue;
            r = this->threads[ i ];
            this->threads[ i ] = 0;
            return r;
        }

        return 0;
    }

    //generate id
    dpid dptaskpool::genId( void )
    {
        dpthread_lock *thd;
        dpid r;

        thd = this->lockThread();
        if( !thd )
            return r;
        r = thd->genId();

        delete thd;
        return r;
    }

    //lock a thread from pool
    dpthread_lock *dptaskpool::lockThread( void )
    {
        dpthread_interface *t;
        dpthread_lock *thd;

        t = this->popThread();
        if( !t )
            return 0;
        this->pushThread( t );

        thd = t->lock();
        return thd;
    }

};
