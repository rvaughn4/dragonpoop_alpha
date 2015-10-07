
#include "root_gui_factory.h"
#include "root_gui.h"

namespace dragonpoop
{
    
    //ctor
    root_gui_factory::root_gui_factory( core *c ) : gui_factory( c )
    {
        
    }
    
    //dtor
    root_gui_factory::~root_gui_factory( void )
    {
        
    }
    
    //generate gui
    gui *root_gui_factory::genGui( gfx_writelock *g, dpid id, dpid pid )
    {
        return new root_gui( g, id );
    }
    
};
