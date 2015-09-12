
#include "model_instance_animation.h"
#include "../../model_animation/model_animation.h"
#include "../../model_readlock.h"
#include "../../model_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include <list>
#include "../../model_animation_frame/model_animation_frame.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_animation::model_instance_animation( model_animation *m ) : model_component( m->getId(), model_component_type_animation )
    {
        this->bIsRepeat = m->isRepeated();
        this->bIsAutplay = m->isAutoPlay();
        this->repeat_delay_f = m->getRepeatDelay();
        this->len_Frames = m->getLength();
        this->cnt_frames = m->getFrameCount();
        this->fps = m->getFps();
        
        this->start_time = this->end_time = 0;
    }
    
    //dtor
    model_instance_animation::~model_instance_animation( void )
    {
        
    }
    
    //set repeat mode
    void model_instance_animation::setRepeated( bool b )
    {
        this->bIsRepeat = b;
    }
    
    //get repeat mode
    bool model_instance_animation::isRepeated( void )
    {
        return this->bIsRepeat;
    }
    
    //set autoplay mode
    void model_instance_animation::setAutoPlay( bool b )
    {
        this->bIsAutplay = b;
    }
    
    //get autoplay mode
    bool model_instance_animation::isAutoPlay( void )
    {
        return this->bIsAutplay;
    }
    
    //get delay between repeats
    unsigned int model_instance_animation::getRepeatDelay( void )
    {
        return this->repeat_delay_f;
    }
    
    //get frame count
    unsigned int model_instance_animation::getFrameCount( void )
    {
        return this->cnt_frames;
    }
    
    //get fps
    float model_instance_animation::getFps( void )
    {
        return this->fps;
    }
    
    //get length of animation in frames
    unsigned int model_instance_animation::getLength( void )
    {
        return this->len_Frames;
    }
    
    
    //start animation
    void model_instance_animation::start( uint64_t t, model_writelock *ml )
    {
        float f;
        f = this->len_Frames * 1000.0f / this->fps;
        this->start_time = t;
        this->end_time = t + (unsigned int)f;
    }
    
    //stop animation
    void model_instance_animation::stop( void )
    {
        this->start_time = this->end_time = 0;
        this->bIsAutplay = 0;
    }
    
    //returns true if playing
    bool model_instance_animation::isPlaying( void )
    {
        return this->start_time != 0;
    }
    
    //return ms played
    uint64_t model_instance_animation::getPlayTime( uint64_t t )
    {
        uint64_t td;
        float f;
        
        if( !this->start_time || !this->end_time )
            return 0;
            
        if( t > this->start_time )
            td = t - this->start_time;
        else
            td = 0;
        
        if( t > this->end_time )
        {
            if( !this->bIsRepeat )
            {
                this->stop();
                return 0;
            }
            this->start_time = this->end_time;
            f = this->len_Frames * 1000.0f / this->fps;
            this->end_time += (unsigned int)f;
            
            return this->getPlayTime( t );
        }
        
        return td;
    }
    
    //convert time into frame number
    unsigned int model_instance_animation::getFrameFromTime( uint64_t tm )
    {
        float f;
        
        f = (float)tm / 1000.0f;
        f *= this->fps;
        
        return (unsigned int)f;
    }
    
    //convert frame number into time
    unsigned int model_instance_animation::getTimeFromFrame( uint64_t tm )
    {
        float f;
        
        f = (float)tm / this->fps;
        f *= 1000.0f;
        
        return (unsigned int)f;
    }
    
    //return closest frame after time
    model_frame *model_instance_animation::findFrameAtTime( model_writelock *ml, uint64_t t, uint64_t *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        t = this->getFrameFromTime( t );
        
        ml->getAnimationFrames( &l, this->getId() );
        
        f = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            if( p->getTime() < t )
                continue;
            if( !f )
            {
                f = p;
                continue;
            }
            if( f->getTime() < p->getTime() )
                continue;
            f = p;
        }
        
        if( !f )
            return 0;
        
        *p_time = this->getTimeFromFrame( f->getTime() );
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }

    //return closest frame before time
    model_frame *model_instance_animation::findFrameBeforeTime( model_writelock *ml, uint64_t t, uint64_t *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        t = this->getFrameFromTime( t );
        
        ml->getAnimationFrames( &l, this->getId() );
        
        f = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            if( p->getTime() > t )
                continue;
            if( !f )
            {
                f = p;
                continue;
            }
            if( f->getTime() > p->getTime() )
                continue;
            f = p;
        }
        
        if( !f )
            return 0;
        
        *p_time = this->getTimeFromFrame( f->getTime() );
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }
    
    //set start frame id
    void model_instance_animation::setStartFrame( dpid id )
    {
        this->start_frame = id;
    }
    
    //get start frame id
    dpid model_instance_animation::getStartFrame( void )
    {
        return this->start_frame;
    }
    
    //set end frame id
    void model_instance_animation::setEndFrame( dpid id )
    {
        this->end_frame = id;
    }
    
    //get end frame id
    dpid model_instance_animation::getEndFrame( void )
    {
        return this->end_frame;
    }
    
};
