
#include "renderer_gui_man_readlock.h"
#include "renderer_gui_man.h"

namespace dragonpoop
{

    //ctor
    renderer_gui_man_readlock::renderer_gui_man_readlock( renderer_gui_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_gui_man_readlock::~renderer_gui_man_readlock( void )
    {

    }

    //return core
    core *renderer_gui_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }

    //get hovering gui id
    dpid renderer_gui_man_readlock::getHoverId( void )
    {
        return this->t->getHoverId();
    }

};
