
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

namespace dragonpoop
{
    
    //ctor
    dpactor_man::dpactor_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        
        this->c = c;
        
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
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0 );
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
        
    }
    
};
