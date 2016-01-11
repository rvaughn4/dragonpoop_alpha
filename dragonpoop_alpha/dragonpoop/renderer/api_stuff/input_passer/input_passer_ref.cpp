
#include "input_passer_ref.h"
#include "input_passer.h"

namespace dragonpoop
{

    //ctor
    input_passer_ref::input_passer_ref( input_passer *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm ) : shared_obj_ref( p, k )
    {
        this->t = p;
        this->tm = tm;
    }

    //dtor
    input_passer_ref::~input_passer_ref( void )
    {

    }

    //get position time
    uint64_t input_passer_ref::getTime( void )
    {
        return *( this->tm.get() );
    }

};
