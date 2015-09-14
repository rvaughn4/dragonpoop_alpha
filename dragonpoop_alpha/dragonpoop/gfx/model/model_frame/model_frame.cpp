
#include "model_frame.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_frame::model_frame( dpid id ) : model_component( id, model_component_type_frame )
    {
        
    }
    
    //dtor
    model_frame::~model_frame( void )
    {
        
    }
    
    //write data to disk/memory
    bool model_frame::writeData( dpbuffer *b )
    {
        model_frame_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }

};
