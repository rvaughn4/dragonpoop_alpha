
#include "openglx_1o5_renderer_gui_ref.h"
#include "openglx_1o5_renderer_gui.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_gui_ref::openglx_1o5_renderer_gui_ref( openglx_1o5_renderer_gui *p, std::shared_ptr<shared_obj_refkernal> *k ) : renderer_gui_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    openglx_1o5_renderer_gui_ref::~openglx_1o5_renderer_gui_ref( void )
    {
        this->unlink();
    }
    
};
