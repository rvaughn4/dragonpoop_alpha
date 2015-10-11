
#include "dpactor_ref.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_ref::dpactor_ref( dpactor *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    dpactor_ref::~dpactor_ref( void )
    {
        
    }
    
};
