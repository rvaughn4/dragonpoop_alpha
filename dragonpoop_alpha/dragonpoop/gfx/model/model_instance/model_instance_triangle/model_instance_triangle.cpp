
#include "model_instance_triangle.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_triangle::model_instance_triangle( model_group_triangle *gt ) : model_component( gt->getId(), model_component_type_triangle )
    {
        this->t_id = gt->getTriangleId();
        this->g_id = gt->getGroupId();
    }
    
    //dtor
    model_instance_triangle::~model_instance_triangle( void )
    {
        
    }
    
    //get triangle id
    dpid model_instance_triangle::getTriangleId( void )
    {
        return this->t_id;
    }
    
    //get vertex id
    dpid model_instance_triangle::getGroupId( void )
    {
        return this->g_id;
    }
    
    //returns true if has parent
    bool model_instance_triangle::hasParent( dpid id )
    {
        if( dpid_compare( &id, &this->g_id ) )
            return 1;
        if( dpid_compare( &id, &this->t_id ) )
            return 1;
        return 0;
    }

};
