
#include "dptaskpool_logger_ref.h"
#include "dptaskpool_logger.h"

namespace dragonpoop
{

    //ctor
    dptaskpool_logger_ref::dptaskpool_logger_ref( dptaskpool_logger *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm ) : shared_obj_ref( p, k )
    {
        this->tm = tm;
        this->t = p;
    }

    //dtor
    dptaskpool_logger_ref::~dptaskpool_logger_ref( void )
    {
    }

    //get position time
    uint64_t dptaskpool_logger_ref::getTime( void )
    {
        return *( this->tm.get() );
    }

};
