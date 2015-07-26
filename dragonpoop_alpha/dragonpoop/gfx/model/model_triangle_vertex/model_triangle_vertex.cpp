
#include "model_triangle_vertex.h"

namespace dragonpoop
{

    //ctor
    model_triangle_vertex::model_triangle_vertex( dpid id ) : model_component( id, model_component_type_triangle_vertex )
    {

    }

    //dtor
    model_triangle_vertex::~model_triangle_vertex( void )
    {

    }

    //set triangle id
    void model_triangle_vertex::setTriangleId( dpid id )
    {
        this->t_id = id;
    }

    //set vertex id
    void model_triangle_vertex::setVertexId( dpid id )
    {
        this->v_id = id;
    }

    //get triangle id
    dpid model_triangle_vertex::getTriangleId( void )
    {
        return this->t_id;
    }

    //get vertex id
    dpid model_triangle_vertex::getVertexId( void )
    {
        return this->v_id;
    }

    //set normals
    void model_triangle_vertex::setNormal( dpxyzw *x )
    {
        this->norm = *x;
    }

    //get normals
    void model_triangle_vertex::getNormal( dpxyzw *x )
    {
        *x = this->norm;
    }

    //set texcoord0
    void model_triangle_vertex::setTexCoord0( dpst *x )
    {
        this->texcoords0 = *x;
    }

    //get texcoord0
    void model_triangle_vertex::getTexCoord0( dpst *x )
    {
        *x = this->texcoords0;
    }

    //set texcoord1
    void model_triangle_vertex::setTexCoord1( dpst *x )
    {
        this->texcoords1 = *x;
    }

    //get texcoord1
    void model_triangle_vertex::getTexCoord1( dpst *x )
    {
        *x = this->texcoords1;
    }

};
