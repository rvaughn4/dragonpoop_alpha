
#include "model_instance_animation.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_animation::model_instance_animation( dpid id ) : model_component( id, model_component_type_animation )
    {
        
    }
    
    //dtor
    model_instance_animation::~model_instance_animation( void )
    {
        
    }
    
    //set repeat mode
    void model_instance_animation::setRepeated( bool b )
    {
        this->bIsRepeat = b;
    }
    
    //get repeat mode
    bool model_instance_animation::isRepeated( void )
    {
        return this->bIsRepeat;
    }
    
    //set autoplay mode
    void model_instance_animation::setAutoPlay( bool b )
    {
        this->bIsAutplay = b;
    }
    
    //get autoplay mode
    bool model_instance_animation::isAutoPlay( void )
    {
        return this->bIsAutplay;
    }
    
    //set delay between repeats
    void model_instance_animation::setRepeatDelay( unsigned int ms )
    {
        this->repeat_delay_ms = ms;
    }
    
    //get delay between repeats
    unsigned int model_instance_animation::getRepeatDelay( void )
    {
        return this->repeat_delay_ms;
    }
    
};