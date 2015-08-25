
#include "model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_group::model_instance_group( model_group *g ) : model_component( g->getId(), model_component_type_group )
    {
        std::string s;
        
        this->m_id = g->getMaterialId();
        
        g->getName( &s );
        this->setName( &s );
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
    
    //set start time
    void model_instance_group::setStartTime( uint64_t t )
    {
        this->t_start = t;
    }
    
    //get start time
    uint64_t model_instance_group::getStartTime( void )
    {
        return this->t_start;
    }
    
    //set end time
    void model_instance_group::setEndTime( uint64_t t )
    {
        this->t_end = t;
    }
    
    //get end time
    uint64_t model_instance_group::getEndTime( void )
    {
        return this->t_end;
    }
    
};
