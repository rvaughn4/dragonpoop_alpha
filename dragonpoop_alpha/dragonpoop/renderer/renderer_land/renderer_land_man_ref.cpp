
#include "renderer_land_man_ref.h"
#include "renderer_land_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_land_man_ref::renderer_land_man_ref( renderer_land_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    renderer_land_man_ref::~renderer_land_man_ref( void )
    {
        
    }
    
};
