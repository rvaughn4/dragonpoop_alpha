
#include "dpthread.h"
#include "dpthread_lock.h"
#include "../dpmutex/dpmutexes.h"
#include "../dptask/dptasks.h"
#include "../dptaskpool/dptaskpools.h"
#include "../shared_obj/shared_obj_guard.h"
#include <stdlib.h>
#include <string.h>
#include <chrono>

namespace dragonpoop
{

    //ctor
    dpthread::dpthread( dpmutex_master *ml, unsigned int id )
    {
        int i;

        this->id = id;
        this->lm = ml;
        this->trun = 1;
        this->l = this->lm->genMutex();
        this->usage = 0;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            this->tasks.dynamic_notran[ i ] = 0;
            this->tasks.dynamic_ran[ i ] = 0;
            this->tasks.static_notran[ i ] = 0;
            this->tasks.static_ran[ i ] = 0;
        }

        this->thd = new std::thread( dpthread_threadproc, this );
    }

    //dtor
    dpthread::~dpthread( void )
    {
        this->kill();
        delete this->thd;
        this->deleteTasks();
        this->lm->destroyMutex( &this->l );
    }

    //add static task
    bool dpthread::addStatic( dptask_ref *t )
    {
        int i;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            if( this->tasks.static_notran[ i ] != 0 )
                continue;
            this->tasks.static_notran[ i ] = t;
            return 1;
        }

        return 0;
    }

    //add dynamic task
    bool dpthread::addDynamic( dptask_ref *t )
    {
        int i;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            if( this->tasks.dynamic_notran[ i ] != 0 )
                continue;
            this->tasks.dynamic_notran[ i ] = t;
            return 1;
        }

        return 0;
    }

    //remove static task
    dptask_ref *dpthread::removeDynamic( void )
    {
        int i;
        dptask_ref *r;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            if( this->tasks.dynamic_notran[ i ] == 0 )
                continue;
            r = this->tasks.dynamic_notran[ i ];
            this->tasks.dynamic_notran[ i ] = 0;
            return r;
        }

        return 0;
    }

    //returns usage
    float dpthread::getUsage( void )
    {
        return this->usage;
    }

    //removes a dynamic task
    dptask_ref *dpthread::getTask( void )
    {
        return this->removeDynamic();
    }

    //returns task count
    unsigned int dpthread::countTasks( void )
    {
        unsigned int i, c;

        c = 0;
        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            if( this->tasks.dynamic_notran[ i ] != 0 )
                c++;
            if( this->tasks.dynamic_ran[ i ] != 0 )
                c++;
            if( this->tasks.static_notran[ i ] != 0 )
                c++;
            if( this->tasks.static_ran[ i ] != 0 )
                c++;
        }

        return c;
    }

    //add new task (creates a ref)
    void dpthread::addTask( dptask_ref *t )
    {
        dptask_ref *at;
        dptask_writelock *tl;
        shared_obj_guard g;
        bool b = 0;

        tl = (dptask_writelock *)g.writeLock( t, "dpthread::addTask" );
        if( !tl )
            return;
        at = (dptask_ref *)tl->getRef();

        if( tl->isSingleThread() )
            b = this->addStatic( at );
        else
            b = this->addDynamic( at );

        if( !b )
            delete at;
    }

    //add new task (creates a ref)
    void dpthread::addTask( dptask *t )
    {
        dptask_ref *at;
        dptask_writelock *tl;
        shared_obj_guard g;
        bool b = 0;

        tl = (dptask_writelock *)g.writeLock( t, "dpthread::addTask" );
        if( !tl )
            return;
        at = (dptask_ref *)tl->getRef();

        if( tl->isSingleThread() )
            b = this->addStatic( at );
        else
            b = this->addDynamic( at );

        if( !b )
            delete at;
    }

    //returns true if has static tasks
    bool dpthread::hasStaticTask( void )
    {
        int i;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            if( this->tasks.static_notran[ i ] != 0 )
                return 1;
            if( this->tasks.static_ran[ i ] != 0 )
                return 1;
        }

        return 0;
    }

    //get tick count in ms
    uint64_t dpthread::getTicks( void )
    {
        return this->ticks;
    }

    //get epoch time in seconds
    uint64_t dpthread::getEpoch( void )
    {
        return this->epoch;
    }

    //lock
    dpthread_lock *dpthread::lock( void )
    {
        dpmutex_writelock *wl;

        wl = this->l->tryWriteLock( "dpthread::lock", 60 );
        if( !wl )
            return 0;

        return new dpthread_lock( this, wl );
    }

    //delete all tasks (or throw them on pool)
    void dpthread::deleteTasks( void )
    {
        dptask_ref *t;
        int i;

        for( i = 0; i < dpthread_max_tasks; i++ )
        {
            t = this->tasks.dynamic_notran[ i ];
            if( t )
                delete t;
            this->tasks.dynamic_notran[ i ] = 0;
            t = this->tasks.dynamic_ran[ i ];
            if( t )
                delete t;
            this->tasks.dynamic_ran[ i ] = 0;
            t = this->tasks.static_notran[ i ];
            if( t )
                delete t;
            this->tasks.static_notran[ i ] = 0;
            t = this->tasks.static_ran[ i ];
            if( t )
                delete t;
            this->tasks.static_ran[ i ] = 0;
        }
    }

    //return id
    unsigned int dpthread::getId( void )
    {
        return this->id;
    }

    //kill thread
    void dpthread::kill( void )
    {
        this->trun = 0;
        if( this->thd->joinable() )
            this->thd->join();
    }

    //generate unique dpid
    dpid dpthread::genId( void )
    {
        dpid r;
        union
        {
            void *p;
            uint64_t pi;
        };
        static unsigned int r0, r1;

        p = (void *)this;
        r.addr = 0 + pi;
        r0 += 1;
        if( r0 > 1 )
        {
            r0 = 0;
            r.addr = 255 - r.addr;
        }
        this->idctr++;
        r.counter = 0 + this->idctr;
        r.epoch = 0 + this->epoch;
        r.rdm = 0 + rand();
        r1 += 1;
        if( r1 > 1 )
        {
            r1 = 0;
            r.rdm = 255 - r.rdm;
        }

        return r;
    }

    //ran by thread
    void dpthread_threadproc( dpthread *t )
    {
        dpthread_lock *tl;
        dptask_ref *r;
        dptask_writelock *rl;
        shared_obj_guard o;
        uint64_t td, lowest_delay;
        int i, j;
        std::chrono::time_point<std::chrono::steady_clock> tp_now;
        std::chrono::steady_clock::duration d_s;

        lowest_delay = 50;
        while( t->trun )
        {
            lowest_delay++;

            tl = t->lock();
            if( !tl )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
                continue;
            }

            for( i = 0; i < dpthread_max_tasks; i++ )
            {

                r = t->tasks.static_ran[ i ];
                t->tasks.static_ran[ i ] = 0;
                if( r )
                {
                    for( j = 0; j < dpthread_max_tasks; j++ )
                    {
                        if( t->tasks.static_notran[ j ] != 0 )
                            continue;
                        t->tasks.static_notran[ j ] = r;
                        j = dpthread_max_tasks;
                    }
                }

                r = t->tasks.dynamic_ran[ i ];
                t->tasks.dynamic_ran[ i ] = 0;
                if( r )
                {
                    for( j = 0; j < dpthread_max_tasks; j++ )
                    {
                        if( t->tasks.dynamic_notran[ j ] != 0 )
                            continue;
                        t->tasks.dynamic_notran[ j ] = r;
                        j = dpthread_max_tasks;
                    }
                }
            }

            tp_now = std::chrono::steady_clock::now();
            d_s = tp_now.time_since_epoch();
            t->ticks = d_s.count() * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
            t->epoch = d_s.count() * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;

            for( i = 0; i < dpthread_max_tasks; i++ )
            {

                r = t->tasks.static_notran[ i ];
                if( r )
                {
                    rl = (dptask_writelock *)o.tryWriteLock( r, 30, "dpthread_threadproc" );
                    if( rl )
                    {
                        td = t->ticks - rl->getLastTime();
                        if( td >= rl->getDelay() )
                        {
                            rl->run( rl, tl );
                            td = rl->getDelay();
                            if( lowest_delay > td )
                                lowest_delay = td;
                            rl->setLastTime( t->ticks );
                            t->tasks.static_notran[ i ] = 0;
                            for( j = 0; j < dpthread_max_tasks; j++ )
                            {
                                if( t->tasks.static_ran[ j ] != 0 )
                                    continue;
                                t->tasks.static_ran[ j ] = r;
                                j = dpthread_max_tasks;
                            }
                        }
                        o.unlock();
                    }
                }

                r = t->tasks.dynamic_notran[ i ];
                if( r )
                {
                    rl = (dptask_writelock *)o.tryWriteLock( r, 30, "dpthread_threadproc" );
                    if( rl )
                    {
                        td = t->ticks - rl->getLastTime();
                        if( td >= rl->getDelay() )
                        {
                            rl->run( rl, tl );
                            td = rl->getDelay();
                            if( lowest_delay > td )
                                lowest_delay = td;
                            rl->setLastTime( t->ticks );
                            t->tasks.dynamic_notran[ i ] = 0;
                            for( j = 0; j < dpthread_max_tasks; j++ )
                            {
                                if( t->tasks.dynamic_ran[ j ] != 0 )
                                    continue;
                                t->tasks.dynamic_ran[ j ] = r;
                                j = dpthread_max_tasks;
                            }
                        }
                        o.unlock();
                    }
                }
            }

            tp_now = std::chrono::steady_clock::now();
            d_s = tp_now.time_since_epoch();
            td = d_s.count() * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
            td -= t->ticks;
            if( lowest_delay < td )
                lowest_delay = td;
            if( lowest_delay )
                t->usage = (float)td / (float)lowest_delay;
            else
                t->usage = 1;
            delete tl;

            td = lowest_delay - td;
            std::this_thread::sleep_for( std::chrono::milliseconds( td ) );
        }

    }

};
