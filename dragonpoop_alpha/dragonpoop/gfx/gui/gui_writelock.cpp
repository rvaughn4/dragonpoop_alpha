
#include "gui_writelock.h"
#include "gui.h"

namespace dragonpoop
{

    //ctor
    gui_writelock::gui_writelock( gui *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_writelock::~gui_writelock( void )
    {
        
    }
    
    //return core
    core *gui_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
