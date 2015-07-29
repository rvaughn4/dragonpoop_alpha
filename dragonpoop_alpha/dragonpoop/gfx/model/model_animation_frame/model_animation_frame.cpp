
#include "model_animation_frame.h"

namespace dragonpoop
{
    
    //ctor
    model_animation_frame::model_animation_frame( dpid id, dpid animation_id, dpid frame_id ) : model_component( id, model_component_type_animation_frame )
    {
        this->animation_id = animation_id;
        this->frame_id = frame_id;
    }
    
    //dtor
    model_animation_frame::~model_animation_frame( void )
    {
        
    }
    
    //return animation id
    dpid model_animation_frame::getAnimationId( void )
    {
        return this->animation_id;
    }
    
    //return frame id
    dpid model_animation_frame::getFrameId( void )
    {
        return this->frame_id;
    }
    
    //set time in ms
    void model_animation_frame::setTime( unsigned int ms )
    {
        this->time_ms = ms;
    }
    
    //get time in ms
    unsigned int model_animation_frame::getTime( void )
    {
        return this->time_ms;
    }

};
