
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

    //set position
    void model_vertex::setPosition( dpxyzw *x )
    {
        *x = this->x;
    }

    //get position
    void model_vertex::getPosition( dpxyzw *x )
    {
        this->x = *x;
    }

};
