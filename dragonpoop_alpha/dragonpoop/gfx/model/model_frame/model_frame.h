
#ifndef dragonpoop_model_frame_h
#define dragonpoop_model_frame_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
#pragma pack( 0 )
    struct model_frame_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_frame_header_v1
    {
        model_frame_header_hdr h;
    };
#pragma pack()
    
    class model_frame : public model_component
    {
        
    private:
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );

    public:
        
        //ctor
        model_frame( dpid id );
        //dtor
        virtual ~model_frame( void );
        
    };
    
};

#endif