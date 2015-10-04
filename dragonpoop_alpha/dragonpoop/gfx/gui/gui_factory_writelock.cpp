
#include "gui_factory_writelock.h"
#include "gui_factory.h"

namespace dragonpoop
{
    
    //ctor
    gui_factory_writelock::gui_factory_writelock( gui_factory *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_factory_writelock::~gui_factory_writelock( void )
    {
        
    }
    
    //make gui
    gui *gui_factory_writelock::makeGui( dpthread_lock *thd, gui *p )
    {
        return this->t->makeGui( thd, p );
    }
    
};
