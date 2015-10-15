
#include "dpactor_man.h"
#include "dpactor_man_ref.h"
#include "dpactor_man_readlock.h"
#include "dpactor_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "dpactor_man_task.h"
#include "../gfx_ref.h"
#include "../gfx.h"
#include "../gfx_writelock.h"
#include "dpactor.h"
#include "dpactor_ref.h"
#include "dpactor_readlock.h"
#include "dpactor_writelock.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_man::dpactor_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        
        this->c = c;
        this->actor_cnt = 0;
        
        gl = (gfx_writelock *)o.writeLock( g, "dpactor_man::dpactor_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();
        
        this->_startTask( tp, 200 );
    }
    
    //dtor
    dpactor_man::~dpactor_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "dpactor_man::~dpactor_man" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "dpactor_man::~dpactor_man" );
        this->_killTask();
        o.unlock();
        
        o.tryWriteLock( this, 5000, "dpactor_man::~dpactor_man" );
        this->_deleteTask();
        delete this->g;
        this->deleteActors();
        o.unlock();
    }
    
    //return core
    core *dpactor_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *dpactor_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpactor_man_readlock( (dpactor_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *dpactor_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpactor_man_writelock( (dpactor_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *dpactor_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpactor_man_ref( (dpactor_man *)p, k );
    }
    
    //start task
    void dpactor_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new dpactor_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "dpactor_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void dpactor_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "dpactor_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void dpactor_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //run
    void dpactor_man::run( dpthread_lock *thd, dpactor_man_writelock *g )
    {
        this->runActors( thd );
    }
    
    //delete all actors
    void dpactor_man::deleteActors( void )
    {
        std::list<dpactor_ref *> *l, d;
        std::list<dpactor_ref *>::iterator i;
        dpactor_ref *p;
        
        l = &this->actors;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }        
    }
    
    //run all actors
    void dpactor_man::runActors( dpthread_lock *thd )
    {
        std::list<dpactor_ref *> *l, ll, d;
        std::list<dpactor_ref *>::iterator i;
        dpactor_ref *p;
        shared_obj_guard o;
        dpactor_writelock *pl;
        
        l = &this->actors;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
            {
                d.push_back( p );
                continue;
            }
            pl = (dpactor_writelock *)o.tryWriteLock( p, 100, "dpactor_man::runActors" );
            if( pl )
                pl->run( thd );
        }
        o.unlock();
        
        if( d.empty() )
            return;
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->actors.remove( p );
            this->actor_cnt--;
            delete p;
        }
    }
    
    //add actor
    void dpactor_man::addActor( dpactor *a )
    {
        shared_obj_guard o;
        dpactor_writelock *l;
        dpactor_ref *r;
        
        l = (dpactor_writelock *)o.writeLock( a, "dpactor_man::addActor" );
        if( !l )
            return;
        
        r = (dpactor_ref *)l->getRef();
        if( r )
        {
            this->actors.push_back( r );
            this->actor_cnt++;
        }
    }
    
    //add actor
    void dpactor_man::addActor( dpactor_ref *a )
    {
        shared_obj_guard o;
        dpactor_writelock *l;
        dpactor_ref *r;
        
        l = (dpactor_writelock *)o.writeLock( a, "dpactor_man::addActor" );
        if( !l )
            return;
        
        r = (dpactor_ref *)l->getRef();
        if( r )
        {
            this->actors.push_back( r );
            this->actor_cnt++;
        }
    }
    
    //return actor count
    unsigned int dpactor_man::getActorCount( void )
    {
        if( this->actor_cnt < 0 )
            return 0;
        return (unsigned int)this->actor_cnt;
    }
    
};
