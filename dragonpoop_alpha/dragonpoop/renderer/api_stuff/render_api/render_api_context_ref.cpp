
#include "render_api_context_ref.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    //ctor
    render_api_context_ref::render_api_context_ref( render_api_context *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_context_ref::~render_api_context_ref( void )
    {
        
    }
    
};
