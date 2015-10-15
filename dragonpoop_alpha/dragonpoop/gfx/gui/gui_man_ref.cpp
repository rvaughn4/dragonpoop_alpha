
#include "gui_man_ref.h"
#include "gui_man.h"

namespace dragonpoop
{
    
    //ctor
    gui_man_ref::gui_man_ref( gui_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    gui_man_ref::~gui_man_ref( void )
    {
        
    }
    
};
