
#include "gui_man_writelock.h"
#include "gui_man.h"

namespace dragonpoop
{
    
    //ctor
    gui_man_writelock::gui_man_writelock( gui_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    gui_man_writelock::~gui_man_writelock( void )
    {
        
    }
    
    //return core
    core *gui_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void gui_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
  
    //add gui
    void gui_man_writelock::addGui( gui *g )
    {
        this->t->addGui( g );
    }
    
    //add gui
    void gui_man_writelock::addGui( gui_ref *g )
    {
        this->t->addGui( g );
    }
    
    //get guis
    void gui_man_writelock::getGuis( std::list<gui_ref *> *l )
    {
        this->t->getGuis( l );
    }
    
    //return gui count
    unsigned int gui_man_writelock::getGuiCount( void )
    {
        return this->t->getGuiCount();
    }
    
    //set root gui factory
    void gui_man_writelock::setRootGui( gui_factory *g )
    {
        this->t->setRootGui( g );
    }
    
};
