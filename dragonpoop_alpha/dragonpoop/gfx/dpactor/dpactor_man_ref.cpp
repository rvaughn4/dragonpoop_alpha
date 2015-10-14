
#include "dpactor_man_ref.h"
#include "dpactor_man.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_man_ref::dpactor_man_ref( dpactor_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    dpactor_man_ref::~dpactor_man_ref( void )
    {
        
    }
    
};
