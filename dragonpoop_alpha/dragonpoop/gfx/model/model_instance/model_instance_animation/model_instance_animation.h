
#ifndef dragonpoop_model_instance_animation_h
#define dragonpoop_model_instance_animation_h

#include "../../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_animation;
    class model_readlock;
    class dpthread_lock;
    class model_frame;
    class model_writelock;
    
    class model_instance_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_f, cnt_frames, len_Frames;
        float fps;
        uint64_t start_time, end_time;
        dpid start_frame, end_frame;
        
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
        //get delay between repeats
        unsigned int getRepeatDelay( void );
        //get frame count
        unsigned int getFrameCount( void );
        //get fps
        float getFps( void );
        //get length of animation in frames
        unsigned int getLength( void );
        //start animation
        void start( uint64_t t, model_writelock *ml );
        //stop animation
        void stop( void );
        //returns true if playing
        bool isPlaying( void );
        //return ms played
        uint64_t getPlayTime( uint64_t tm );
        //convert time into frame number
        unsigned int getFrameFromTime( uint64_t tm );
        //convert frame number into time
        unsigned int getTimeFromFrame( uint64_t tm );
        //return closest frame after time
        model_frame *findFrameAtTime( model_writelock *ml, uint64_t t, uint64_t *p_time );
        //return closest frame before time
        model_frame *findFrameBeforeTime( model_writelock *ml, uint64_t t, uint64_t *p_time );
        //set start frame id
        void setStartFrame( dpid id );
        //get start frame id
        dpid getStartFrame( void );
        //set end frame id
        void setEndFrame( dpid id );
        //get end frame id
        dpid getEndFrame( void );

    };
    
};

#endif