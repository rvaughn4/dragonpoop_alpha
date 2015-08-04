
#ifndef dragonpoop_model_instance_animation_h
#define dragonpoop_model_instance_animation_h

#include "../../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_instance_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_ms;
        
    protected:
        
    public:
        
        //ctor
        model_instance_animation( dpid id );
        //dtor
        virtual ~model_instance_animation( void );
        //set repeat mode
        void setRepeated( bool b );
        //get repeat mode
        bool isRepeated( void );
        //set autoplay mode
        void setAutoPlay( bool b );
        //get autoplay mode
        bool isAutoPlay( void );
        //set delay between repeats
        void setRepeatDelay( unsigned int ms );
        //get delay between repeats
        unsigned int getRepeatDelay( void );
        
    };
    
};

#endif