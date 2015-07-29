
#include "model_group_triangle.h"

namespace dragonpoop
{

    //ctor
    model_group_triangle::model_group_triangle( dpid id, dpid group_id, dpid triangle_id ) : model_component( id, model_component_type_group_triangle )
    {
        this->g_id = group_id;
        this->t_id = triangle_id;
    }

    //dtor
    model_group_triangle::~model_group_triangle( void )
    {

    }

    //get triangle id
    dpid model_group_triangle::getTriangleId( void )
    {
        return this->t_id;
    }

    //get group id
    dpid model_group_triangle::getGroupId( void )
    {
        return this->g_id;
    }

};
