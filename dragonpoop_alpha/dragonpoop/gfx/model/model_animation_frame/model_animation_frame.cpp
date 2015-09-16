
#include "model_animation_frame.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_animation_frame::model_animation_frame( dpid id, dpid animation_id, dpid frame_id ) : model_component( id, model_component_type_animation_frame )
    {
        this->animation_id = animation_id;
        this->frame_id = frame_id;
    }
    
    //dtor
    model_animation_frame::~model_animation_frame( void )
    {
        
    }
    
    //return animation id
    dpid model_animation_frame::getAnimationId( void )
    {
        return this->animation_id;
    }
    
    //return frame id
    dpid model_animation_frame::getFrameId( void )
    {
        return this->frame_id;
    }
    
    //set time in ms
    void model_animation_frame::setTime( unsigned int ms )
    {
        this->time_ms = ms;
    }
    
    //get time in ms
    unsigned int model_animation_frame::getTime( void )
    {
        return this->time_ms;
    }
    
    //returns true if has parent
    bool model_animation_frame::hasParent( dpid id )
    {
        return dpid_compare( &this->frame_id, &id ) || dpid_compare( &this->animation_id, &id );
    }
    
    //write data to disk/memory
    bool model_animation_frame::writeData( dpbuffer *b )
    {
        model_animation_frame_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.animation_id = this->animation_id;
        h.frame_id = this->frame_id;
        h.time_ms = this->time_ms;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
    //read data from disk/memory
    bool model_animation_frame::readData( dpbuffer *b )
    {
        model_animation_frame_header_v1 h;
        unsigned int rc;
        
        rc = b->getReadCursor();
        if( !b->readBytes( (uint8_t *)&h, sizeof( h.h ) ) )
            return 0;
        b->setReadCursor( rc );
        
        if( h.h.version < 1 || h.h.size < sizeof( h ) )
            return 0;
        if( !b->readBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        b->setReadCursor( rc + h.h.size );
        
        this->animation_id = h.animation_id;
        this->frame_id = h.frame_id;
        this->time_ms = h.time_ms;
        
        return 1;
    }

};
