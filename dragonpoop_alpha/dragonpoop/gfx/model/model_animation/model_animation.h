
#ifndef dragonpoop_model_animation_h
#define dragonpoop_model_animation_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_readlock;
    class model_writelock;
    
    class model_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_f, cnt_frames, len_frames;
        float speed;
        
    protected:
        
    public:
        
        //ctor
        model_animation( dpid id );
        //dtor
        virtual ~model_animation( void );
        //set repeat mode
        void setRepeated( bool b );
        //get repeat mode
        bool isRepeated( void );
        //set autoplay mode
        void setAutoPlay( bool b );
        //get autoplay mode
        bool isAutoPlay( void );
        //set delay between repeats
        void setRepeatDelay( unsigned int f );
        //get delay between repeats
        unsigned int getRepeatDelay( void );
        //get frame count
        unsigned int getFrameCount( void );
        //refresh frame count
        void countFrames( model_readlock *m );
        //refresh frame count
        void countFrames( model_writelock *m );
        //set play speed fps
        void setFps( float s );
        //get fps
        float getFps( void );
        //set length of animation in frames
        void setLength( unsigned int f );
        //get length of animation in frames
        unsigned int getLength( void );
        
    };
    
};

#endif