
#include "dptaskpool.h"
#include "dptaskpool_ref.h"
#include "dptaskpool_readlock.h"
#include "dptaskpool_writelock.h"
#include "../dptask/dptasks.h"
#include "../dpthread/dpthreads.h"
#include "../shared_obj/shared_obj_guard.h"
#include "dptaskpool_task.h"
#include "dptaskpool_logger_writelock.h"

#include <thread>
#include <vector>

namespace dragonpoop
{

    //ctor
    dptaskpool::dptaskpool( dpmutex_master *mm, unsigned int thread_cnt ) : shared_obj( mm )
    {
        int i;
        dpthread_lock *tl;

        this->mm = mm;

        for( i = 0; i < dptaskpool_max_tasks; i++ )
            this->tasks[ i ] = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
            this->threads[ i ] = 0;

        if( !thread_cnt )
            thread_cnt = std::thread::hardware_concurrency();
        if( thread_cnt < 3 )
            thread_cnt = 3;

        this->lggr = new dptaskpool_logger( mm );

        this->setThreadCount( thread_cnt );

        this->tptsk = new dptaskpool_task( this );
        this->tsk = new dptask( this->mm, this->tptsk, 1000, 1, "task pool manager" );

        tl = this->lockThread();
        if( !tl )
            return;
        tl->addTask( this->tsk );
        delete tl;
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
        delete this->lggr;
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

    //run taskpool
    void dptaskpool::run( dptask_writelock *tskl, dpthread_lock *th )
    {
        unsigned int i;
        dpthread_interface *t;
        dpthread_lock *tl;
        dptask_ref *r;
        dptask_readlock *rl;
        shared_obj_guard o;
        bool b;
        std::vector<dptask_ref *> skipped;

        r = this->popTask();
        while( r )
        {
            rl = (dptask_readlock *)o.tryReadLock( r, 10, "dptaskpool::run" );
            if( !rl )
                skipped.push_back( r );
            else
            {
                b = rl->isSingleThread();
                o.unlock();

                if( b )
                {
                    t = this->getThread_noStatic();
                    if( !t )
                        b = 0;
                    else
                    {
                        tl = t->lock();
                        if( !tl )
                            b = 0;
                        else
                        {
                            tl->addTask( r );
                            delete tl;
                        }
                    }

                }

                if( !b )
                {
                    t = this->getThread_fewestTasks();
                    if( !t )
                        skipped.push_back( r );
                    else
                    {
                        tl = t->lock();
                        if( !tl )
                            skipped.push_back( r );
                        else
                        {
                            tl->addTask( r );
                            delete r;
                            delete tl;
                        }
                    }
                }
            }
            r = this->popTask();
        }

        for( i = 0; i < skipped.size(); i++ )
        {
            r = skipped[ i ];
            this->pushTask( r );
        }
        skipped.clear();

        t = this->getThread_highUsage();
        if( !t )
            return;
        tl = t->lock();
        if( !tl )
            return;
        r = tl->getTask();
        delete tl;

        if( !r )
            return;

        t = this->getThread_lowUsage();
        if( !t )
        {
            this->pushTask( r );
            return;
        }
        tl = t->lock();
        if( !tl )
        {
            this->pushTask( r );
            return;
        }
        tl->addTask( r );
        delete r;
        delete tl;

        this->updateLogger( th );
    }

    //find thread with fewest tasks
    dpthread_interface *dptaskpool::getThread_fewestTasks( void )
    {
        unsigned int i, mtc, tc;
        dpthread_interface *r, *t;
        dpthread_lock *tl;

        mtc = 2000;
        r = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] == 0 )
                continue;
            t = this->threads[ i ];
            tl = t->lock();
            if( !tl )
                continue;
            tc = tl->countTasks();
            if( tc < mtc )
            {
                mtc = tc;
                r = t;
            }
            delete tl;
        }

        return r;
    }

    //find thread with no static tasks
    dpthread_interface *dptaskpool::getThread_noStatic( void )
    {
        unsigned int i;
        dpthread_interface *t;
        dpthread_lock *tl;

        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] == 0 )
                continue;
            t = this->threads[ i ];
            tl = t->lock();
            if( !tl )
                continue;

            if( !tl->hasStaticTask() )
            {
                delete tl;
                return t;
            }

            delete tl;
        }

        return 0;
    }

    //find thread with highest usage
    dpthread_interface *dptaskpool::getThread_highUsage( void )
    {
        unsigned int i;
        dpthread_interface *r, *t;
        dpthread_lock *tl;
        float mtc, tc;

        mtc = 0;
        r = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] == 0 )
                continue;
            t = this->threads[ i ];
            tl = t->lock();
            if( !tl )
                continue;
            tc = tl->getUsage();
            if( tc > mtc )
            {
                mtc = tc;
                r = t;
            }
            delete tl;
        }

        return r;
    }

    //find thread with lowest usage
    dpthread_interface *dptaskpool::getThread_lowUsage( void )
    {
        unsigned int i;
        dpthread_interface *r, *t;
        dpthread_lock *tl;
        float mtc, tc;

        mtc = 2;
        r = 0;
        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            if( this->threads[ i ] == 0 )
                continue;
            t = this->threads[ i ];
            tl = t->lock();
            if( !tl )
                continue;
            tc = tl->getUsage();
            if( tc < mtc )
            {
                mtc = tc;
                r = t;
            }
            delete tl;
        }

        return r;
    }

    //update logger
    void dptaskpool::updateLogger( dpthread_lock *thdl )
    {
        dpthread_lock *thd;
        dpthread_interface *t;
        unsigned int i;
        dptaskpool_logger_writelock *l;
        shared_obj_guard o;

        l = (dptaskpool_logger_writelock *)o.tryWriteLock( this->lggr, 100, "dptaskpool::updateLogger" );
        if( !l )
            return;

        l->clear();

        for( i = 0; i < dptaskpool_max_threads; i++ )
        {
            t = this->threads[ i ];
            if( !t )
                continue;
            thd = t->lock();
            if( !thd )
                continue;
            this->updateLogger( thd, l );
            delete thd;
        }

        l->setTime( thdl->getTicks() );
    }

    //update logger per thread
    void dptaskpool::updateLogger( dpthread_lock *thd, dptaskpool_logger_writelock *lggr )
    {
        std::atomic<dptask_ref *> *l;
        unsigned int i, e;
        dptask_ref *p;

        e = thd->getDynamicTaskRanList( &l );
        for( i = 0; i < e; i++ )
        {
            p = l[ i ];
            if( !p )
                continue;
            lggr->addTask( thd->getId(), p );
        }

        e = thd->getStaticTaskRanList( &l );
        for( i = 0; i < e; i++ )
        {
            p = l[ i ];
            if( !p )
                continue;
            lggr->addTask( thd->getId(), p );
        }

        e = thd->getDynamicTaskNotRanList( &l );
        for( i = 0; i < e; i++ )
        {
            p = l[ i ];
            if( !p )
                continue;
            lggr->addTask( thd->getId(), p );
        }

        e = thd->getStaticTaskNotRanList( &l );
        for( i = 0; i < e; i++ )
        {
            p = l[ i ];
            if( !p )
                continue;
            lggr->addTask( thd->getId(), p );
        }
    }

    //get logger
    dptaskpool_logger_ref *dptaskpool::getLogger( void )
    {
        dptaskpool_logger_writelock *l;
        shared_obj_guard o;

        l = (dptaskpool_logger_writelock *)o.tryWriteLock( this->lggr, 100, "dptaskpool::getLogger" );
        if( !l )
            return 0;
        return (dptaskpool_logger_ref *)l->getRef();
    }

};
