
#include "model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_group::model_instance_group( dpid id ) : model_component( id, model_component_type_group )
    {
        dpid_zero( &this->m_id );
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
    
};
