
#include "dpposition_share_ref.h"
#include "dpposition_share.h"

namespace dragonpoop
{

    //ctor
    dpposition_share_ref::dpposition_share_ref( dpposition_share *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm ) : shared_obj_ref( p, k )
    {
        this->t = p;
        this->tm = tm;
    }

    //dtor
    dpposition_share_ref::~dpposition_share_ref( void )
    {

    }

    //get position time
    uint64_t dpposition_share_ref::getTime( void )
    {
        return *(this->tm.get() );
    }

};
