
#ifndef dragonpoop_model_animation_frame_h
#define dragonpoop_model_animation_frame_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
#pragma pack( 0 )
    struct model_animation_frame_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_animation_frame_header_v1
    {
        model_animation_frame_header_hdr h;
        dpid animation_id, frame_id;
        uint32_t time_ms;
    };
#pragma pack()

    class model_animation_frame : public model_component
    {
        
    private:
        
        unsigned int time_ms;
        dpid animation_id, frame_id;
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );
        //read data from disk/memory
        virtual bool readData( dpbuffer *b );

    public:
        
        //ctor
        model_animation_frame( dpid id, dpid animation_id, dpid frame_id );
        //dtor
        virtual ~model_animation_frame( void );
        //return animation id
        dpid getAnimationId( void );
        //return frame id
        dpid getFrameId( void );
        //set time in ms
        void setTime( unsigned int ms );
        //get time in ms
        unsigned int getTime( void );
        //returns true if has parent
        virtual bool hasParent( dpid id );
        
    };
    
};

#endif