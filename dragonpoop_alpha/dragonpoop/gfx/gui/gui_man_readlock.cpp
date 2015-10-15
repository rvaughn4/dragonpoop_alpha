
#include "gui_man_readlock.h"
#include "gui_man.h"

namespace dragonpoop
{
    
    //ctor
    gui_man_readlock::gui_man_readlock( gui_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_man_readlock::~gui_man_readlock( void )
    {
        
    }
    
    //return core
    core *gui_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get guis
    void gui_man_readlock::getGuis( std::list<gui_ref *> *l )
    {
        this->t->getGuis( l );
    }
    
    //return gui count
    unsigned int gui_man_readlock::getGuiCount( void )
    {
        return this->t->getGuiCount();
    }
    
};
