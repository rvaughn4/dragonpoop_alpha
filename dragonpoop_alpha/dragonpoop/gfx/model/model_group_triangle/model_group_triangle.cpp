
#include "model_group_triangle.h"

namespace dragonpoop
{

    //ctor
    model_group_triangle::model_group_triangle( dpid id ) : model_component( id, model_component_type_group_triangle )
    {

    }

    //dtor
    model_group_triangle::~model_group_triangle( void )
    {

    }

    //set triangle id
    void model_group_triangle::setTriangleId( dpid id )
    {
        this->t_id = id;
    }

    //set group id
    void model_group_triangle::setGroupId( dpid id )
    {
        this->g_id = id;
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
