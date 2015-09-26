
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
    class model_instance_writelock;
    
    class model_instance_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay, bDoPlay, bIsPlay;
        unsigned int repeat_delay_f, cnt_frames, len_Frames, current_frame_time, start_frame_time, end_frame_time;
        float fps, speed;
        uint64_t start_time, end_time, current_time;
        dpid start_frame, end_frame, old_end_frame;
        dpid anim_id;
        
    protected:
        
    public:
        
        //ctor
        model_instance_animation( dpid id, model_animation *m );
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
        void start( void );
        //stop animation
        void stop( void );
        //returns true if playing
        bool isPlaying( void );
        //convert time into frame number
        unsigned int getFrameFromTime( uint64_t tm );
        //convert frame number into time
        unsigned int getTimeFromFrame( uint64_t tm );
        //get start frame id
        dpid getStartFrame( void );
        //get end frame id
        dpid getEndFrame( void );
        //run animation
        void run( model_instance_writelock *mi, model_writelock *m, dpthread_lock *thd, unsigned int frame_time );
        //return end frame time
        uint64_t getEndTime( void );
        //return start frame time
        uint64_t getStartTime( void );
        //return animation id
        dpid getAnimationId( void );
        //set speed
        void setSpeed( float f );

    };
    
};

#endif