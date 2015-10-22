
#include "render_api_indexbuffer_ref.h"
#include "render_api_indexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_indexbuffer_ref::render_api_indexbuffer_ref( render_api_indexbuffer *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_indexbuffer_ref::~render_api_indexbuffer_ref( void )
    {
        
    }
    
};
