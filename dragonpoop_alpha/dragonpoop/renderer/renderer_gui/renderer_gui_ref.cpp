
#include "renderer_gui_ref.h"
#include "renderer_gui.h"

namespace dragonpoop
{
    
    //ctor
    renderer_gui_ref::renderer_gui_ref( renderer_gui *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    renderer_gui_ref::~renderer_gui_ref( void )
    {
        this->unlink();
    }
    
};
