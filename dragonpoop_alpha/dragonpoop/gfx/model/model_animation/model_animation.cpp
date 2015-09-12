
#include "model_animation.h"
#include "../model_readlock.h"
#include "../model_writelock.h"

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
    
};
