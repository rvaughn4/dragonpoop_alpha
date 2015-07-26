
#include "model_group.h"

namespace dragonpoop
{

    //ctor
    model_group::model_group( dpid id ) : model_component( id, model_component_type_group )
    {
        dpid_zero( &this->m_id );
    }

    //dtor
    model_group::~model_group( void )
    {

    }

    //set material id
    void model_group::setMaterialId( dpid id )
    {
        this->m_id = id;
    }

    //get material id
    dpid model_group::getMaterialId( void )
    {
        return this->m_id;
    }

};
