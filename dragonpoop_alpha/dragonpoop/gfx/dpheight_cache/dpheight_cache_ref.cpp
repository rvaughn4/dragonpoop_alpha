
#include "dpheight_cache_ref.h"
#include "dpheight_cache.h"

namespace dragonpoop
{

    //ctor
    dpheight_cache_ref::dpheight_cache_ref( dpheight_cache *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm ) : shared_obj_ref( p, k )
    {
        this->t = p;
        this->tm = tm;
    }

    //dtor
    dpheight_cache_ref::~dpheight_cache_ref( void )
    {

    }

    //get position time
    uint64_t dpheight_cache_ref::getTime( void )
    {
        return *( this->tm.get() );
    }

};
