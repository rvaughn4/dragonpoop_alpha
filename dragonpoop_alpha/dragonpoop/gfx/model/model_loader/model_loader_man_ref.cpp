
#include "model_loader_man_ref.h"
#include "model_loader_man.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_man_ref::model_loader_man_ref( model_loader_man *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    model_loader_man_ref::~model_loader_man_ref( void )
    {
        
    }
    
};
