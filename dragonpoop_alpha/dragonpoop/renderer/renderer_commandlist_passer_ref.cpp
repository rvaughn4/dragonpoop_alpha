
#include "renderer_commandlist_passer_ref.h"
#include "renderer_commandlist_passer.h"

namespace dragonpoop
{
    
    //ctor
    renderer_commandlist_passer_ref::renderer_commandlist_passer_ref( renderer_commandlist_passer *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    renderer_commandlist_passer_ref::~renderer_commandlist_passer_ref( void )
    {
        this->unlink();
    }
    
};
