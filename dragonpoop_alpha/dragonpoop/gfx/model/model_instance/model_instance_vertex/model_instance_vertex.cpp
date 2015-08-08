
#include "model_instance_vertex.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_vertex::model_instance_vertex( model_vertex *v ) : model_component( v->getId(), model_component_type_vertex )
    {
        v->getPosition( &this->pos );
        this->start.pos = this->end.pos = this->pos;
        this->start.t = this->end.t = 0;
    }
    
    //dtor
    model_instance_vertex::~model_instance_vertex( void )
    {
        
    }
    
    //get position
    void model_instance_vertex::getPosition( dpxyzw *x )
    {
        *x = this->pos;
    }
    
    //set position
    void model_instance_vertex::setPosition( dpxyzw *x )
    {
        this->pos = *x;
    }
    
};
