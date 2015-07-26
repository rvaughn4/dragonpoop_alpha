
#include "model_ref.h"
#include "model.h"

namespace dragonpoop
{

    //ctor
    model_ref::model_ref( model *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }

    //dtor
    model_ref::~model_ref( void )
    {
        this->unlink();
    }
    
};
