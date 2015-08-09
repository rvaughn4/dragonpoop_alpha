
#include "openglx_1o5_renderer_model_instance_ref.h"
#include "openglx_1o5_renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_ref::openglx_1o5_renderer_model_instance_ref( openglx_1o5_renderer_model_instance *p, std::shared_ptr<shared_obj_refkernal> *k ) : renderer_model_instance_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_ref::~openglx_1o5_renderer_model_instance_ref( void )
    {
        
    }
    
};
