
#include "gui_readlock.h"
#include "gui.h"

namespace dragonpoop
{
    
    //ctor
    gui_readlock::gui_readlock( gui *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_readlock::~gui_readlock( void )
    {
        
    }
    
    //return core
    core *gui_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
