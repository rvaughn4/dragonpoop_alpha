
#include "dptask_ref.h"
#include "dptask.h"

namespace dragonpoop
{

    //ctor
    dptask_ref::dptask_ref( dptask *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    dptask_ref::~dptask_ref( void )
    {
        this->unlink();
    }

};
