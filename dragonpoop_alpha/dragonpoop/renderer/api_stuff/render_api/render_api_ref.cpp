
#include "render_api_ref.h"
#include "render_api.h"

namespace dragonpoop
{
    
    //ctor
    render_api_ref::render_api_ref( render_api *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_ref::~render_api_ref( void )
    {
        
    }
    
};
