
#include "renderer_model_man_ref.h"
#include "renderer_model_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man_ref::renderer_model_man_ref( renderer_model_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    renderer_model_man_ref::~renderer_model_man_ref( void )
    {
        
    }
    
};
