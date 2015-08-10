
#include "model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_group::model_instance_group( model_group *g ) : model_component( g->getId(), model_component_type_group )
    {
        this->m_id = g->getMaterialId();
    }
    
    //dtor
    model_instance_group::~model_instance_group( void )
    {
        
    }
    
    //set material id
    void model_instance_group::setMaterialId( dpid id )
    {
        this->m_id = id;
    }
    
    //get material id
    dpid model_instance_group::getMaterialId( void )
    {
        return this->m_id;
    }
    
    //get vertex buffer
    dpvertexindex_buffer *model_instance_group::getVertexBuffer()
    {
        return &this->vb;
    }
    
};
