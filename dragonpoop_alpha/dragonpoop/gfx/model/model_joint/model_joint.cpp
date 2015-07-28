
#include "model_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_joint::model_joint( dpid id ) : model_component( id, model_component_type_joint )
    {
        
    }
    
    //dtor
    model_joint::~model_joint( void )
    {
        
    }
    
    //set position
    void model_joint::setPosition( dpxyzw *x )
    {
        this->pos = *x;
    }
    
    //get position
    void model_joint::getPosition( dpxyzw *x )
    {
        *x = this->pos;
    }

};
