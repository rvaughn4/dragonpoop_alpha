
#include "model_vertex_joint.h"

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

};
