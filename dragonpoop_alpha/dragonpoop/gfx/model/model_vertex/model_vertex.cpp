
#include "model_vertex.h"

namespace dragonpoop
{

    //ctor
    model_vertex::model_vertex( dpid id ) : model_component( id, model_component_type_vertex )
    {

    }

    //dtor
    model_vertex::~model_vertex( void )
    {

    }

    //get position
    void model_vertex::getPosition( dpxyz_f *x )
    {
        *x = this->x;
    }

    //set position
    void model_vertex::setPosition( dpxyz_f *x )
    {
        this->x = *x;
    }

    //set position
    void model_vertex::setPosition( model_vector *v )
    {
        v->getPosition( &this->x );
    }
    
    //get position
    void model_vertex::getPosition( model_vector *v )
    {
        v->setPosition( &this->x );
    }
    
};
