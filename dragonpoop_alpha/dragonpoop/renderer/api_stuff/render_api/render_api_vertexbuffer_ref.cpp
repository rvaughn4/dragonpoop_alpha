
#include "render_api_vertexbuffer_ref.h"
#include "render_api_vertexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_vertexbuffer_ref::render_api_vertexbuffer_ref( render_api_vertexbuffer *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_vertexbuffer_ref::~render_api_vertexbuffer_ref( void )
    {
        
    }
    
};
