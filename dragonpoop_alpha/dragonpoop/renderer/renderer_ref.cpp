
#include "renderer_ref.h"
#include "renderer.h"

namespace dragonpoop
{

    //ctor
    renderer_ref::renderer_ref( renderer *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    renderer_ref::~renderer_ref( void )
    {
        this->unlink();
    }
    
};
