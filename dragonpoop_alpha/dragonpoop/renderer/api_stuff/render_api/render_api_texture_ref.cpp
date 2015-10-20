
#include "render_api_texture_ref.h"
#include "render_api_texture.h"

namespace dragonpoop
{
    
    //ctor
    render_api_texture_ref::render_api_texture_ref( render_api_texture *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_texture_ref::~render_api_texture_ref( void )
    {
        
    }
    
};
