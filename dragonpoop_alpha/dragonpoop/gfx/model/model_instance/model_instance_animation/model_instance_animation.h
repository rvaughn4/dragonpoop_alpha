
#ifndef dragonpoop_model_instance_animation_h
#define dragonpoop_model_instance_animation_h

#include "../../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_animation;
    class model_readlock;
    class dpthread_lock;
    class model_frame;
    
    class model_instance_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_ms;
        uint64_t start_time, end_time;
        
    protected:
        
    public:
        
        //ctor
        model_instance_animation( model_animation *m );
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
        //start animation
        void start( dpthread_lock *thd, model_readlock *ml );
        //stop animation
        void stop( void );
        //returns true if playing
        bool isPlaying( void );
        //return ms played
        uint64_t getPlayTime( dpthread_lock *thd );
        //return closest frame after time
        model_frame *getFrameAtTime( model_readlock *ml, uint64_t t, uint64_t *p_time );
        //return closest frame before time
        model_frame *getFrameBeforeTime( model_readlock *ml, uint64_t t, uint64_t *p_time );
        
    };
    
};

#endif