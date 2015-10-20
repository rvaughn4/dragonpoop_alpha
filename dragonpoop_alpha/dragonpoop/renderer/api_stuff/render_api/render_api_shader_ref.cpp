
#include "render_api_shader_ref.h"
#include "render_api_shader.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader_ref::render_api_shader_ref( render_api_shader *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_shader_ref::~render_api_shader_ref( void )
    {
        
    }
    
};
