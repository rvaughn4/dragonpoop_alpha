
#include "gfx_ref.h"
#include "gfx.h"

namespace dragonpoop
{

    //ctor
    gfx_ref::gfx_ref( gfx *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    gfx_ref::~gfx_ref( void )
    {
        this->unlink();
    }
    
};
