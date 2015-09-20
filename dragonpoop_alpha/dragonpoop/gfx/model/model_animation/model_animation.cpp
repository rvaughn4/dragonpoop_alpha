
#include "model_animation.h"
#include "../model_readlock.h"
#include "../model_writelock.h"
#include "../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_animation::model_animation( dpid id ) : model_component( id, model_component_type_animation )
    {
        this->repeat_delay_f = 0;
        this->bIsAutplay = 0;
        this->bIsRepeat = 0;
        this->cnt_frames = 0;
        this->speed = 30.0f;
    }
    
    //dtor
    model_animation::~model_animation( void )
    {
        
    }
    
    //set repeat mode
    void model_animation::setRepeated( bool b )
    {
        this->bIsRepeat = b;
    }
    
    //get repeat mode
    bool model_animation::isRepeated( void )
    {
        return this->bIsRepeat;
    }
    
    //set autoplay mode
    void model_animation::setAutoPlay( bool b )
    {
        this->bIsAutplay = b;
    }
    
    //get autoplay mode
    bool model_animation::isAutoPlay( void )
    {
        return this->bIsAutplay;
    }
    
    //set delay between repeats
    void model_animation::setRepeatDelay( unsigned int f )
    {
        this->repeat_delay_f = f;
    }
    
    //get delay between repeats
    unsigned int model_animation::getRepeatDelay( void )
    {
        return this->repeat_delay_f;
    }
    
    //get frame count
    unsigned int model_animation::getFrameCount( void )
    {
        return this->cnt_frames;
    }
    
    //refresh frame count
    void model_animation::countFrames( model_readlock *m )
    {
        std::list<model_animation_frame *> l;
        
        m->getAnimationFrames( &l, this->getId() );
        
        this->cnt_frames = (unsigned int)l.size();
    }
    
    //refresh frame count
    void model_animation::countFrames( model_writelock *m )
    {
        std::list<model_animation_frame *> l;
        
        m->getAnimationFrames( &l, this->getId() );
        
        this->cnt_frames = (unsigned int)l.size();
    }
    
    //set play speed fps
    void model_animation::setFps( float s )
    {
        this->speed = s;
    }
    
    //get fps
    float model_animation::getFps( void )
    {
        return this->speed;
    }
  
    //set length of animation in frames
    void model_animation::setLength( unsigned int f )
    {
        this->len_frames = f;
    }
    
    //get length of animation in frames
    unsigned int model_animation::getLength( void )
    {
        return this->len_frames;
    }
    
    //write data to disk/memory
    bool model_animation::writeData( dpbuffer *b )
    {
        model_animation_header_v1 h;
        
        h.h.version = 1;
        h.h.size = sizeof( h );
        h.bIsAutplay = this->bIsAutplay;
        h.bIsRepeat = this->bIsRepeat;
        h.cnt_frames = this->cnt_frames;
        h.len_frames = this->len_frames;
        h.speed = this->speed;
        h.repeat_delay_f = this->repeat_delay_f;
        
        return b->writeBytes( (uint8_t *)&h, sizeof( h ) );
    }
    
    //read data from disk/memory
    bool model_animation::readData( dpbuffer *b )
    {
        model_animation_header_v1 h;
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
        
        this->bIsAutplay = h.bIsAutplay;
        this->bIsRepeat = h.bIsRepeat;
        this->cnt_frames = h.cnt_frames;
        this->len_frames = h.len_frames;
        this->speed = h.speed;
        this->repeat_delay_f = h.repeat_delay_f;
        
        return 1;
    }

};
