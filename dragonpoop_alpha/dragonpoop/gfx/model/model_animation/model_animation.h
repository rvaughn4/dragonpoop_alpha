
#ifndef dragonpoop_model_animation_h
#define dragonpoop_model_animation_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
    class model_readlock;
    class model_writelock;
    class model_frame;
    
#pragma pack( 0 )
    struct model_animation_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_animation_header_v1
    {
        model_animation_header_hdr h;
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_f, cnt_frames, len_frames;
        float speed;
    };
#pragma pack()
    
    class model_animation : public model_component
    {
        
    private:
        
        bool bIsRepeat, bIsAutplay;
        unsigned int repeat_delay_f, cnt_frames, len_frames;
        float speed;
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );
        //read data from disk/memory
        virtual bool readData( dpbuffer *b );

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
        //reduce frame resolution by eliminating frames
        void reduceFrames( model_writelock *ml, unsigned int ms_res );
        //return closest frame after time
        model_frame *findFrameAtTime( model_writelock *ml, unsigned int t, unsigned int *p_time );
        //return closest frame before time
        model_frame *findFrameBeforeTime( model_writelock *ml, unsigned int t, unsigned int *p_time );
        //returns frame with largest movement within a frame time range
        model_frame *findBiggestFrame( model_writelock *ml, unsigned int t_start, unsigned int t_end );
        //returns frame with smallest movement within a frame time range
        model_frame *findSmallestFrame( model_writelock *ml, unsigned int t_start, unsigned int t_end );
        //eliminate all frames smaller than given weight
        void killSmallerFrames( model_writelock *ml, model_frame *f, unsigned int t_start, unsigned int t_end );

    };
    
};

#endif