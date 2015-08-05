
#include "model_instance_triangle_vertex.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_triangle_vertex::model_instance_triangle_vertex( model_triangle_vertex *v ) : model_component( v->getId(), model_component_type_triangle_vertex )
    {
        this->t_id = v->getTriangleId();
        this->v_id = v->getVertexId();
    }
    
    //dtor
    model_instance_triangle_vertex::~model_instance_triangle_vertex( void )
    {
        
    }
    
    //get triangle id
    dpid model_instance_triangle_vertex::getTriangleId( void )
    {
        return this->t_id;
    }
    
    //get vertex id
    dpid model_instance_triangle_vertex::getVertexId( void )
    {
        return this->v_id;
    }
    
    //set normals
    void model_instance_triangle_vertex::setNormal( dpxyzw *x )
    {
        this->norm = *x;
    }
    
    //get normals
    void model_instance_triangle_vertex::getNormal( dpxyzw *x )
    {
        *x = this->norm;
    }
    
    //set texcoord0
    void model_instance_triangle_vertex::setTexCoord0( dpst *x )
    {
        this->texcoords0 = *x;
    }
    
    //get texcoord0
    void model_instance_triangle_vertex::getTexCoord0( dpst *x )
    {
        *x = this->texcoords0;
    }
    
    //set texcoord1
    void model_instance_triangle_vertex::setTexCoord1( dpst *x )
    {
        this->texcoords1 = *x;
    }
    
    //get texcoord1
    void model_instance_triangle_vertex::getTexCoord1( dpst *x )
    {
        *x = this->texcoords1;
    }
    
    //returns true if has parent
    bool model_instance_triangle_vertex::hasParent( dpid id )
    {
        if( dpid_compare( &id, &this->t_id ) )
            return 1;
        if( dpid_compare( &id, &this->v_id ) )
            return 1;
        return 0;
    }
    
};
