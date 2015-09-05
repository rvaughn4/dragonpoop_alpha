
#include "model_saver_ref.h"
#include "model_saver.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ref::model_saver_ref( model_saver *p, std::shared_ptr<shared_obj_refkernal> *k ) : shared_obj_ref( p, k )
    {
        this->t = p;
    }
    
    //dtor
    model_saver_ref::~model_saver_ref( void )
    {
        
    }
    
};
