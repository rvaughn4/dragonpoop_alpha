
#include "model_triangle.h"

namespace dragonpoop
{

    //ctor
    model_triangle::model_triangle( dpid id ) : model_component( id, model_component_type_triangle )
    {

    }

    //dtor
    model_triangle::~model_triangle( void )
    {

    }

    //set vertex 0 id
    void model_triangle::setVertex0( dpid id )
    {
        this->v0 = id;
    }

    //set vertex 1 id
    void model_triangle::setVertex1( dpid id )
    {
        this->v1 = id;
    }

    //set vertex 2 id
    void model_triangle::setVertex2( dpid id )
    {
        this->v2 = id;
    }

    //get vertex 0 id
    dpid model_triangle::getVertex0( void )
    {
        return this->v0;
    }

    //get vertex 1 id
    dpid model_triangle::getVertex1( void )
    {
        return this->v1;
    }

    //get vertex 2 id
    dpid model_triangle::getVertex2( void )
    {
        return this->v2;
    }

};
