
#include "model_frame_joint.h"
#include "../../../core/dpbuffer/dpbuffer.h"

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
    void model_frame_joint::setRotation( dpxyz_f *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_frame_joint::getRotation( dpxyz_f *x )
    {
        *x = this->rot;
    }
    
    //set translation
    void model_frame_joint::setTranslation( dpxyz_f *x )
    {
        this->trans = *x;
    }
    
    //get translation
    void model_frame_joint::getTranslation( dpxyz_f *x )
    {
        *x = this->trans;
    }
    
    //returns true if has parent
    bool model_frame_joint::hasParent( dpid id )
    {
        return dpid_compare( &id, &this->joint_id ) || dpid_compare( &id, &this->frame_id );
    }
    
    //write data to disk/memory
    bool model_frame_joint::writeData( dpbuffer *b )
    {
        model_frame_joint_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.frame_id = this->frame_id;
        h.joint_id = this->joint_id;
        h.rot = this->rot;
        h.trans = this->trans;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }

};
