
#include "renderer_gui_man_writelock.h"
#include "renderer_gui_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_gui_man_writelock::renderer_gui_man_writelock( renderer_gui_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_gui_man_writelock::~renderer_gui_man_writelock( void )
    {
        
    }
    
    //return core
    core *renderer_gui_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run from manager thread
    void renderer_gui_man_writelock::runFromTask( dpthread_lock *thd )
    {
        this->t->runFromTask( thd, this );
    }
    
    //run from renderer thread
    void renderer_gui_man_writelock::runFromRenderer( dpthread_lock *thd )
    {
        this->t->runFromRenderer( thd, this );
    }
    
};
