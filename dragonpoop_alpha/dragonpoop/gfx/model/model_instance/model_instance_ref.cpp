
#include "model_instance_ref.h"
#include "model_instance.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_ref::model_instance_ref( model_instance *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    model_instance_ref::~model_instance_ref( void )
    {
        
    }
    
};
