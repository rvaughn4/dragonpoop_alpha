
#ifndef dragonpoop_model_instance_triangle_h
#define dragonpoop_model_instance_triangle_h

#include "../../model_component/model_components.h"
#include "../../model_group_triangle/model_group_triangle.h"

namespace dragonpoop
{
    
    class model_instance_triangle : public model_component
    {
        
    private:
        
        dpid g_id, t_id;
        
    protected:
        
    public:
        
        //ctor
        model_instance_triangle( model_group_triangle *gt );
        //dtor
        virtual ~model_instance_triangle( void );
        //get triangle id
        dpid getTriangleId( void );
        //get vertex id
        dpid getGroupId( void );
        //returns true if has parent
        virtual bool hasParent( dpid id );
        
    };
    
};

#endif