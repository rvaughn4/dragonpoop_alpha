
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
    void model_vertex::getPosition( dpxyzw *x )
    {
        *x = this->x;
    }

    //set position
    void model_vertex::setPosition( dpxyzw *x )
    {
        this->x = *x;
    }

};
