
#include "dpland_man.h"
#include "dpland_man_ref.h"
#include "dpland_man_readlock.h"
#include "dpland_man_writelock.h"
#include "../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    dpland_man::dpland_man( core *c ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        
    }
    
    //dtor
    dpland_man::~dpland_man( void )
    {
        
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
