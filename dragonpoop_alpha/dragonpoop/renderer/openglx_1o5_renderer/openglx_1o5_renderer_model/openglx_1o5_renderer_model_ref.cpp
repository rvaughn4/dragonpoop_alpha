
#include "openglx_1o5_renderer_model_ref.h"
#include "openglx_1o5_renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_ref::openglx_1o5_renderer_model_ref( openglx_1o5_renderer_model *p, std::shared_ptr<shared_obj_refkernal> *k ) : renderer_model_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    openglx_1o5_renderer_model_ref::~openglx_1o5_renderer_model_ref( void )
    {
        
    }
    
};
