
#include "renderer_model_group_instance.h"
#include "../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_group::renderer_model_instance_group( model_instance_group *g ) : model_component( g->getId(), model_component_type_group )
    {
        this->m_id = g->getMaterialId();
    }
    
    //dtor
    renderer_model_instance_group::~renderer_model_instance_group( void )
    {
        
    }
    
    //set material id
    void renderer_model_instance_group::setMaterialId( dpid id )
    {
        this->m_id = id;
    }
    
    //get material id
    dpid renderer_model_instance_group::getMaterialId( void )
    {
        return this->m_id;
    }

};
