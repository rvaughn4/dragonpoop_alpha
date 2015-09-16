
#include "model_vertex_joint.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_vertex_joint::model_vertex_joint( dpid id, dpid vertex_id, dpid joint_id ) : model_component( id, model_component_type_vertex_joint )
    {
        this->vertex_id = vertex_id;
        this->joint_id = joint_id;
    }
    
    //dtor
    model_vertex_joint::~model_vertex_joint( void )
    {
        
    }
    
    //get vertex id
    dpid model_vertex_joint::getVertexId( void )
    {
        return this->vertex_id;
    }
    
    //get joint id
    dpid model_vertex_joint::getJointId( void )
    {
        return this->joint_id;
    }
    
    //get weight
    float model_vertex_joint::getWeight( void )
    {
        return this->w;
    }
    
    //set weight
    void model_vertex_joint::setWeight( float w )
    {
        this->w = w;
    }
    
    //write data to disk/memory
    bool model_vertex_joint::writeData( dpbuffer *b )
    {
        model_vertex_joint_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.joint_id = this->joint_id;
        h.vertex_id = this->vertex_id;
        h.w = this->w;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
    //read data from disk/memory
    bool model_vertex_joint::readData( dpbuffer *b )
    {
        model_vertex_joint_header_v1 h;
        unsigned int rc;
        
        rc = b->getReadCursor();
        if( !b->readBytes( (uint8_t *)&h, sizeof( h.h ) ) )
            return 0;
        b->setReadCursor( rc );
        
        if( h.h.version < 1 || h.h.size < sizeof( h ) )
            return 0;
        if( !b->readBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        b->setReadCursor( rc + h.h.size );
        
        this->joint_id = h.joint_id;
        this->vertex_id = h.vertex_id;
        this->w = h.w;
        
        return 1;
    }

};
