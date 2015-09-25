
#include "gui.h"
#include "gui_ref.h"
#include "gui_readlock.h"
#include "gui_writelock.h"
#include "../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    gui::gui( core *c, dpid id ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->id = id;
    }
    
    //dtor
    gui::~gui( void )
    {
        
    }
    
    //return core
    core *gui::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gui_readlock( (gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gui_writelock( (gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gui_ref( (gui *)p, k );
    }
    
    //run gui
    void gui::run( dpthread_lock *thd, gui_writelock *g )
    {
        
    }
    
    //returns id
    dpid gui::getId( void )
    {
        return this->id;
    }
    
    //compares id
    bool gui::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }
    
};
