
#include "model_frame_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_frame_joint::model_frame_joint( dpid id, dpid frame_id, dpid joint_id ) : model_component( id, model_component_type_frame_joint )
    {
        this->frame_id = frame_id;
        this->joint_id = joint_id;
    }
    
    //dtor
    model_frame_joint::~model_frame_joint( void )
    {
        
    }
    
    //return frame id
    dpid model_frame_joint::getFrameId( void )
    {
        return this->frame_id;
    }
    
    //return joint id
    dpid model_frame_joint::getJointId( void )
    {
        return this->joint_id;
    }
    
    //set rotation
    void model_frame_joint::setRotation( dpxyzw *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_frame_joint::getRotation( dpxyzw *x )
    {
        *x = this->rot;
    }
    
    //set translation
    void model_frame_joint::setTranslation( dpxyzw *x )
    {
        this->trans = *x;
    }
    
    //get translation
    void model_frame_joint::getTranslation( dpxyzw *x )
    {
        *x = this->trans;
    }

};
