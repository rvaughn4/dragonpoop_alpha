
#include "gui_factory_readlock.h"
#include "gui_factory.h"

namespace dragonpoop
{
    
    //ctor
    gui_factory_readlock::gui_factory_readlock( gui_factory *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_factory_readlock::~gui_factory_readlock( void )
    {
        
    }
    
    //make gui
    gui *gui_factory_readlock::makeGui( dpthread_lock *thd, gui *p )
    {
        return this->t->makeGui( thd, p );
    }
    
};
