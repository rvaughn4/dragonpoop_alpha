
#include "dpsky_man_ref.h"
#include "dpsky_man.h"

namespace dragonpoop
{
    
    //ctor
    dpsky_man_ref::dpsky_man_ref( dpsky_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    dpsky_man_ref::~dpsky_man_ref( void )
    {
        
    }
    
};
