
#include "gui_factory_ref.h"
#include "gui_factory.h"

namespace dragonpoop
{
    
    //ctor
    gui_factory_ref::gui_factory_ref( gui_factory *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    gui_factory_ref::~gui_factory_ref( void )
    {
        
    }
    
};
