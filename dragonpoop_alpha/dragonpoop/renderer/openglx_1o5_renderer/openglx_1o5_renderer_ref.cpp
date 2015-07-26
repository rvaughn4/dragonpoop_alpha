
#include "openglx_1o5_renderer_ref.h"
#include "openglx_1o5_renderer.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_ref::openglx_1o5_renderer_ref( openglx_1o5_renderer *p, std::shared_ptr<shared_obj_refkernal> *k ) : renderer_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    openglx_1o5_renderer_ref::~openglx_1o5_renderer_ref( void )
    {

    }
    
};
