
#include "model_man_ref.h"
#include "model_man.h"

namespace dragonpoop
{
    
    //ctor
    model_man_ref::model_man_ref( model_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    model_man_ref::~model_man_ref( void )
    {
        
    }
    
};
