
#include "openglx_1o5_renderer_gui_man.h"
#include "openglx_1o5_renderer_gui.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_gui_man::openglx_1o5_renderer_gui_man( core *c, openglx_1o5_renderer *r, dptaskpool_writelock *tp ) : renderer_gui_man( c, r, tp )
    {
        this->r = r;
    }
    
    //dtor
    openglx_1o5_renderer_gui_man::~openglx_1o5_renderer_gui_man( void )
    {
        this->deleteGuis();
    }
    
    //generate renderer gui
    renderer_gui *openglx_1o5_renderer_gui_man::genGui( gui_writelock *ml )
    {
        return new openglx_1o5_renderer_gui( ml, this->r );
    }
    
};
