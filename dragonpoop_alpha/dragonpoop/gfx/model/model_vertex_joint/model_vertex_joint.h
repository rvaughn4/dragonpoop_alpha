
#ifndef dragonpoop_model_vertex_joint_h
#define dragonpoop_model_vertex_joint_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_vertex_joint : public model_component
    {
        
    private:
        
        dpid vertex_id, joint_id;
        float w;
        
    protected:
        
    public:
        
        //ctor
        model_vertex_joint( dpid id, dpid vertex_id, dpid joint_id );
        //dtor
        virtual ~model_vertex_joint( void );
        //get vertex id
        dpid getVertexId( void );
        //get joint id
        dpid getJointId( void );
        //get weight
        float getWeight( void );
        //set weight
        void setWeight( float w );
        
    };
    
};

#endif