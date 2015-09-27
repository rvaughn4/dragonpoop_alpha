
#include "openglx_1o5_renderer_gui_writelock.h"
#include "openglx_1o5_renderer_gui.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_gui_writelock::openglx_1o5_renderer_gui_writelock( openglx_1o5_renderer_gui *t, dpmutex_writelock *l ) : renderer_gui_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_gui_writelock::~openglx_1o5_renderer_gui_writelock( void )
    {
        
    }
    
};
