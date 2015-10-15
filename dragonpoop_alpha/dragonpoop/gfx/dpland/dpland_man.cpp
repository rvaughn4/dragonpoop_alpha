
#include "dpland_man.h"
#include "dpland_man_ref.h"
#include "dpland_man_readlock.h"
#include "dpland_man_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "dpland_man_task.h"
#include "../gfx_ref.h"
#include "../gfx.h"
#include "../gfx_writelock.h"

namespace dragonpoop
{
    
    //ctor
    dpland_man::dpland_man( core *c, gfx *g, dptaskpool_writelock *tp ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        gfx_writelock *gl;
        
        this->c = c;
        
        gl = (gfx_writelock *)o.writeLock( g, "dpland_man::dpland_man" );
        this->g = (gfx_ref *)gl->getRef();
        o.unlock();
        
        this->_startTask( tp, 500 );
    }
    
    //dtor
    dpland_man::~dpland_man( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        this->_killTask();
        o.unlock();

        o.tryWriteLock( this, 5000, "dpland_man::~dpland_man" );
        this->_deleteTask();
        delete this->g;
        o.unlock();
    }
    
    //return core
    core *dpland_man::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *dpland_man::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpland_man_readlock( (dpland_man *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *dpland_man::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpland_man_writelock( (dpland_man *)p, l );
    }
    
    //generate ref
    shared_obj_ref *dpland_man::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpland_man_ref( (dpland_man *)p, k );
    }
    
    //start task
    void dpland_man::_startTask( dptaskpool_writelock *tp, unsigned int ms_delay )
    {
        this->gtsk = new dpland_man_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, ms_delay, 0, "dpland_man" );
        tp->addTask( this->tsk );
    }
    
    //kill task
    void dpland_man::_killTask( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;
        
        if( !this->tsk )
            return;
        
        tl = (dptask_writelock *)o.writeLock( this->tsk, "dpland_man::_killTask" );
        tl->kill();
        o.unlock();
    }
    
    //delete task
    void dpland_man::_deleteTask( void )
    {
        if( this->tsk )
            delete this->tsk;
        this->tsk = 0;
        if( this->gtsk )
            delete this->gtsk;
        this->gtsk = 0;
    }
    
    //run
    void dpland_man::run( dpthread_lock *thd, dpland_man_writelock *g )
    {
        
    }
    
    //run tiles
    void dpland_man::runTiles( dpthread_lock *thd, dpland_man_writelock *g )
    {
        
    }
    
    //create new tiles
    void dpland_man::makeTiles( dpthread_lock *thd, dpland_man_writelock *g )
    {
        
    }
    
    //returns true if tile exists
    bool dpland_man::hasTile( int64_t x, int64_t z )
    {
        return 1;
    }
    
};
