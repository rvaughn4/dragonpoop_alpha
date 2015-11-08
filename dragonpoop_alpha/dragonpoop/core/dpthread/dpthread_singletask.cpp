
#include "dpthread_singletask.h"
#include "dpthread_lock.h"
#include "../dpmutex/dpmutexes.h"
#include "../dptask/dptasks.h"
#include "../shared_obj/shared_obj_guard.h"
#include <stdlib.h>
#include <string.h>
#include <chrono>

namespace dragonpoop
{
    
    //ctor
    dpthread_singletask::dpthread_singletask( dpmutex_master *ml, unsigned int id )
    {
        this->id = id;
        this->lm = ml;
        this->l = this->lm->genMutex();
        this->tsk = 0;
        this->trun = 1;
        this->tisrun = 1;
        this->thd = new std::thread( dpthread_st_threadproc, this );
    }
    
    //dtor
    dpthread_singletask::~dpthread_singletask( void )
    {
        this->kill();
        delete this->thd;
        if( this->tsk )
            delete this->tsk;
        this->lm->destroyMutex( &this->l );
    }
    
    //add new task (creates a ref)
    void dpthread_singletask::addTask( dptask_ref *t )
    {
        dptask_writelock *tl;
        shared_obj_guard g;
        
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        
        tl = (dptask_writelock *)g.writeLock( t, "dpthread_singletask::addTask" );
        if( !tl )
            return;
        this->tsk = (dptask_ref *)tl->getRef();
    }
    
    //add new task (creates a ref)
    void dpthread_singletask::addTask( dptask *t )
    {
        dptask_writelock *tl;
        shared_obj_guard g;
        
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        
        tl = (dptask_writelock *)g.writeLock( t, "dpthread_singletask::addTask" );
        if( !tl )
            return;
        this->tsk = (dptask_ref *)tl->getRef();
    }
    
    //get tick count in ms
    uint64_t dpthread_singletask::getTicks( void )
    {
        return this->ticks;
    }
    
    //get epoch time in seconds
    uint64_t dpthread_singletask::getEpoch( void )
    {
        return this->epoch;
    }
    
    //lock
    dpthread_lock *dpthread_singletask::lock( void )
    {
        dpmutex_writelock *wl;
        
        wl = this->l->tryWriteLock( "dpthread_singletask::lock", 30 );
        if( !wl )
            return 0;
        
        return new dpthread_lock( this, wl );
    }
    
    //return id
    unsigned int dpthread_singletask::getId( void )
    {
        return this->id;
    }
    
    //kill thread
    void dpthread_singletask::kill( void )
    {
        this->trun = 0;
        if( this->thd->joinable() )
            this->thd->join();
        while( this->tisrun )
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    
    //generate unique dpid
    dpid dpthread_singletask::genId( void )
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
    
    //add task pool
    void dpthread_singletask::addPool( dptaskpool *tp )
    {
        
    }
    
    //ran by thread
    void dpthread_st_threadproc( dpthread_singletask *t )
    {
        dpthread_lock *tl;
        dptask_writelock *rl;
        shared_obj_guard g;
        uint64_t td, tt;
        
        t->tisrun = 1;
        while( t->trun )
        {
            tl = t->lock();
            if( !tl )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }
            
            std::chrono::time_point<std::chrono::steady_clock> tp_now;
            tp_now = std::chrono::steady_clock::now();
            std::chrono::steady_clock::duration d_s = tp_now.time_since_epoch();
            
            t->ticks = d_s.count() * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
            t->epoch = d_s.count() * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
            
            if( !t->tsk )
            {
                delete tl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }
            
            if( !t->tsk->isLinked() )
            {
                delete t->tsk;
                t->tsk = 0;
                delete tl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }
            
            rl = (dptask_writelock *)g.tryWriteLock( t->tsk, 30, "dpthread_singletask_threadproc" );
            if( !rl )
            {
                delete tl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
                continue;
            }
            
            td = rl->getDelay();
            tt = rl->getLastTime();
            if( tt > t->ticks )
                tt = 0;
            if( t->ticks - tt < td )
            {
                delete tl;
                g.unlock();
                tt = td - ( t->ticks - tt );
                std::this_thread::sleep_for( std::chrono::milliseconds( tt ) );
                continue;
            }

            if( !rl->isAlive() )
            {
                g.unlock();
                delete t->tsk;
                t->tsk = 0;
                delete tl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                continue;
            }
            
            rl->run( rl, tl );
            rl->setLastTime( t->ticks );
            g.unlock();
            delete tl;
            std::this_thread::sleep_for( std::chrono::milliseconds( td ) );
        }
        
        t->tisrun = 0;
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

    }
    
};
