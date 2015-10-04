
#include "gui_factory.h"
#include "gui_factory_ref.h"
#include "gui_factory_readlock.h"
#include "gui_factory_writelock.h"
#include "gui.h"
#include "gui_writelock.h"
#include "../../core/core.h"
#include "../gfx_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    gui_factory::gui_factory( core *c ) : shared_obj( c->getMutexMaster() )
    {
        this->g = c->getGfx();
    }
    
    //dtor
    gui_factory::~gui_factory( void )
    {
        shared_obj_writelock *l;
        shared_obj_guard o;
        
        l = o.tryWriteLock( this, 2000, "gui_factory::~gui_factory" );
        
        this->unlink();
        delete this->g;

        o.unlock();
    }
    
    //generate read lock
    shared_obj_readlock *gui_factory::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gui_factory_readlock( (gui_factory *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *gui_factory::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gui_factory_writelock( (gui_factory *)p, l );
    }
    
    //generate ref
    shared_obj_ref *gui_factory::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gui_factory_ref( (gui_factory *)p, k );
    }
    
    //make gui
    gui *gui_factory::makeGui( dpthread_lock *thd, gui *p )
    {
        gui *r;
        gui_writelock *l;
        shared_obj_guard o;
        gfx_writelock *fl;
        
        fl = (gfx_writelock *)o.tryWriteLock( this->g, 2000, "gui_factory::makeGui" );
        if( !fl )
            return 0;
        
        r = this->genGui( fl, thd->genId() );
        if( !r || !p )
            return r;
        
        l = (gui_writelock *)o.tryWriteLock( r, 2000, "gui_factory::makeGui" );
        if( !l )
            return r;
        l->addGui( r );
        
        return r;
    }
    
    //generate gui
    gui *gui_factory::genGui( gfx_writelock *g, dpid id )
    {
        return 0;
    }
    
};
