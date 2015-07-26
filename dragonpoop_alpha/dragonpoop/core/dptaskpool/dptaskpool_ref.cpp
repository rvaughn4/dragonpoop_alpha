
#include "dptaskpool_ref.h"
#include "dptaskpool.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_ref::dptaskpool_ref( dptaskpool *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    dptaskpool_ref::~dptaskpool_ref( void )
    {

    }

};
