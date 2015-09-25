
#include "gui_ref.h"
#include "gui.h"

namespace dragonpoop
{
    
    //ctor
    gui_ref::gui_ref( gui *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    gui_ref::~gui_ref( void )
    {
        this->unlink();
    }
    
};
