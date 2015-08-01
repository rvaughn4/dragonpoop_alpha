
#include "model_loader_ref.h"
#include "model_loader.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ref::model_loader_ref( model_loader *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    model_loader_ref::~model_loader_ref( void )
    {
        
    }

};
