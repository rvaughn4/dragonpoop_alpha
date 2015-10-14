
#include "dpland_man_ref.h"
#include "dpland_man.h"

namespace dragonpoop
{
    
    //ctor
    dpland_man_ref::dpland_man_ref( dpland_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    dpland_man_ref::~dpland_man_ref( void )
    {
        
    }
    
};
