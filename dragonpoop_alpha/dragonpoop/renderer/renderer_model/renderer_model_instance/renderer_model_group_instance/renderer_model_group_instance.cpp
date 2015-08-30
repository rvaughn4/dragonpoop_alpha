
#include "renderer_model_group_instance.h"
#include "../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"
#include "../../../../gfx/model/model_instance/model_instance_writelock.h"


namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_group::renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g ) : model_component( g->getId(), model_component_type_group )
    {
        this->sync( ml, g );
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

    //sync with group
    void renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g )
    {
        this->m_id = g->getMaterialId();
        this->t_start = ml->getStartTime();
        this->t_end = ml->getEndTime();
    }
    
    //sync with group
    void renderer_model_instance_group::animate( model_instance_writelock *ml, model_instance_group *g )
    {
        this->t_start = ml->getStartTime();
        this->t_end = ml->getEndTime();
    }
    
    //get start time
    uint64_t renderer_model_instance_group::getStartTime( void )
    {
        return this->t_start;
    }
    
    //get end time
    uint64_t renderer_model_instance_group::getEndTime( void )
    {
        return this->t_end;
    }
    
};
