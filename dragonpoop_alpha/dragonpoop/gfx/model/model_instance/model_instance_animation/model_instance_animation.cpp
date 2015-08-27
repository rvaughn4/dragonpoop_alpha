
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
        this->repeat_delay_ms = m->getRepeatDelay();
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
    
    //set delay between repeats
    void model_instance_animation::setRepeatDelay( unsigned int ms )
    {
        this->repeat_delay_ms = ms;
    }
    
    //get delay between repeats
    unsigned int model_instance_animation::getRepeatDelay( void )
    {
        return this->repeat_delay_ms;
    }
    
    //start animation
    void model_instance_animation::start( uint64_t t, model_writelock *ml )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p;
        
        this->start_time = t;
        this->end_time = 0;
        
        ml->getAnimationFrames( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( p->getTime() > this->end_time )
                this->end_time = p->getTime();
        }
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
        
        if( !this->start_time || !this->end_time )
            return 0;
            
        if( t > this->start_time )
            td = t - this->start_time;
        else
            td = 0;
        
        if( td > this->end_time )
        {
            if( !this->bIsRepeat )
            {
                this->stop();
                return 0;
            }
            this->start_time = t + this->repeat_delay_ms - ( td - this->end_time );
            if( t > this->start_time )
                td = t - this->start_time;
            else
                td = 0;
        }
        
        return td;
    }
    
    //return closest frame after time
    model_frame *model_instance_animation::findFrameAtTime( model_writelock *ml, uint64_t t, uint64_t *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        ml->getAnimationFrames( &l );
        
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
        
        *p_time = f->getTime();
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }

    //return closest frame before time
    model_frame *model_instance_animation::findFrameBeforeTime( model_writelock *ml, uint64_t t, uint64_t *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        ml->getAnimationFrames( &l );
        
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
        
        *p_time = f->getTime();
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
