
#ifndef dragonpoop_model_group_triangle_h
#define dragonpoop_model_group_triangle_h

#include "../model_component/model_components.h"

namespace dragonpoop
{

    class model_group_triangle : public model_component
    {

    private:

        dpid g_id, t_id;

    protected:

    public:

        //ctor
        model_group_triangle( dpid id );
        //dtor
        virtual ~model_group_triangle( void );
        //set triangle id
        void setTriangleId( dpid id );
        //set group id
        void setGroupId( dpid id );
        //get triangle id
        dpid getTriangleId( void );
        //get group id
        dpid getGroupId( void );

    };
    
};

#endif