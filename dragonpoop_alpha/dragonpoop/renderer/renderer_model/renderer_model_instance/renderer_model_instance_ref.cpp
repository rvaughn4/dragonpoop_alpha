
#include "renderer_model_instance_ref.h"
#include "renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_ref::renderer_model_instance_ref( renderer_model_instance *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    renderer_model_instance_ref::~renderer_model_instance_ref( void )
    {
        
    }
    
};
