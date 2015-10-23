
#include "render_api_commandlist_ref.h"
#include "render_api_commandlist.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_ref::render_api_commandlist_ref( render_api_commandlist *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    render_api_commandlist_ref::~render_api_commandlist_ref( void )
    {
        
    }
    
};
