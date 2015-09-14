
#include "model_triangle_vertex.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{

    //ctor
    model_triangle_vertex::model_triangle_vertex( dpid id, dpid triangle_id, dpid vertex_id ) : model_component( id, model_component_type_triangle_vertex )
    {
        this->t_id = triangle_id;
        this->v_id = vertex_id;
    }

    //dtor
    model_triangle_vertex::~model_triangle_vertex( void )
    {

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
    void model_triangle_vertex::setNormal( dpxyz_f *x )
    {
        this->norm = *x;
    }

    //get normals
    void model_triangle_vertex::getNormal( dpxyz_f *x )
    {
        *x = this->norm;
    }
    
    //set normals
    void model_triangle_vertex::setNormal( model_vector *v )
    {
        v->getPosition( &this->norm );
    }
    
    //get normals
    void model_triangle_vertex::getNormal( model_vector *v )
    {
        v->setPosition( &this->norm );
    }

    //set texcoord0
    void model_triangle_vertex::setTexCoord0( dpst_f *x )
    {
        this->texcoords0 = *x;
    }

    //get texcoord0
    void model_triangle_vertex::getTexCoord0( dpst_f *x )
    {
        *x = this->texcoords0;
    }

    //set texcoord1
    void model_triangle_vertex::setTexCoord1( dpst_f *x )
    {
        this->texcoords1 = *x;
    }

    //get texcoord1
    void model_triangle_vertex::getTexCoord1( dpst_f *x )
    {
        *x = this->texcoords1;
    }
    
    //returns true if has parent
    bool model_triangle_vertex::hasParent( dpid id )
    {
        if( dpid_compare( &id, &this->t_id ) )
            return 1;
        if( dpid_compare( &id, &this->v_id ) )
            return 1;
        return 0;
    }

    //write data to disk/memory
    bool model_triangle_vertex::writeData( dpbuffer *b )
    {
        model_triangle_vertex_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.v_id = this->v_id;
        h.t_id = this->t_id;
        h.norm = this->norm;
        h.texcoords0 = this->texcoords0;
        h.texcoords1 = this->texcoords1;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
};
