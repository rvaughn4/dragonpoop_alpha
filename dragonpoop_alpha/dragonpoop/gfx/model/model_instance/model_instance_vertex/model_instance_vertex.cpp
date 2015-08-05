
#include "model_instance_vertex.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_vertex::model_instance_vertex( model_vertex *v ) : model_component( v->getId(), model_component_type_vertex )
    {
        v->getPosition( &this->x );
    }
    
    //dtor
    model_instance_vertex::~model_instance_vertex( void )
    {
        
    }
    
    //get position
    void model_instance_vertex::getPosition( dpxyzw *x )
    {
        *x = this->x;
    }
    
    //set position
    void model_instance_vertex::setPosition( dpxyzw *x )
    {
        this->x = *x;
    }
    
};
