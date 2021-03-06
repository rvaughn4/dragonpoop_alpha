
#include "model_instance_animation.h"
#include "../../model_animation/model_animation.h"
#include "../../model_readlock.h"
#include "../../model_writelock.h"
#include "../model_instance_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include <list>
#include "../../model_animation_frame/model_animation_frame.h"
#include "../../model_frame/model_frame.h"
#include "../../../dpvertex/dpxyz_f.h"
#include "../../model_frame_joint/model_frame_joint.h"
#include <math.h>

namespace dragonpoop
{
    
    //ctor
    model_instance_animation::model_instance_animation( dpid id, model_animation *m ) : model_component( id, model_component_type_animation )
    {
        std::string s;
        
        this->anim_id = m->getId();
        this->bIsRepeat = m->isRepeated();
        this->bIsAutplay = m->isAutoPlay();
        this->repeat_delay_f = m->getRepeatDelay();
        this->len_Frames = m->getLength();
        this->cnt_frames = m->getFrameCount();
        this->fps = m->getFps();
        this->speed = 1;
        this->bIsPlay = 0;
        this->bDoPlay = this->bIsAutplay;
        this->start_time = this->end_time = 0;
        this->end_frame_time = this->start_frame_time = 0;
        dpid_zero( &this->start_frame );
        this->old_end_frame = this->end_frame = this->start_frame;
        
        m->getName( &s );
        this->setName( &s );
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
    void model_instance_animation::start( void )
    {
        this->bDoPlay = 1;
    }
    
    //stop animation
    void model_instance_animation::stop( void )
    {
        this->bDoPlay = 0;
    }
    
    //returns true if playing
    bool model_instance_animation::isPlaying( void )
    {
        return this->bIsPlay;
    }
    
    //convert time into frame number
    unsigned int model_instance_animation::getFrameFromTime( uint64_t tm )
    {
        float f;
        
        f = (float)tm / 1000.0f;
        f *= this->fps * this->speed;
        
        return (unsigned int)f;
    }
    
    //convert frame number into time
    unsigned int model_instance_animation::getTimeFromFrame( uint64_t tm )
    {
        float f;
        
        f = (float)tm / ( this->fps * this->speed );
        f *= 1000.0f;
        
        return (unsigned int)f;
    }
    
    //get start frame id
    dpid model_instance_animation::getStartFrame( void )
    {
        return this->start_frame;
    }
    
    //get end frame id
    dpid model_instance_animation::getEndFrame( void )
    {
        return this->end_frame;
    }
    
    //run animation
    void model_instance_animation::run( model_instance_writelock *mi, model_writelock *m, dpthread_lock *thd, unsigned int frame_time )
    {
        float f;
        model_frame *frm;
        model_animation *a;
        
        //stop playing
        if( !this->bDoPlay )
        {
            this->bIsPlay = 0;
            return;
        }
        
        //is animation done?
        this->current_time = thd->getTicks();
        if( this->bIsPlay && this->current_time > this->end_time )
        {
            this->bDoPlay = this->bIsRepeat;
            this->bIsPlay = 0;
            dpid_zero( &this->start_frame );
            this->start_frame_time = 0;
            if( !this->bDoPlay )
                return;
        }
        
        //start playing
        if( !this->bIsPlay )
        {
            this->start_time = this->current_time;
            this->current_frame_time = 0;
            f = (float)this->len_Frames * 1000.0f / ( this->fps * this->speed );
            this->end_time = this->start_time + (uint64_t)f;
            this->bIsPlay = 1;
        }
        
        a = m->findAnimation( this->getAnimationId() );
        if( !a )
            return;
        
        //calc current animation frame
        f = (float)this->current_time - (float)this->start_time;
        f = f * this->fps * this->speed / 1000.0f;
        this->current_frame_time = (unsigned int)f;
        this->start_frame_time = this->current_frame_time;
        this->end_frame_time = this->current_frame_time + frame_time;
        if( this->bIsRepeat && this->end_frame_time > this->len_Frames )
        {
            this->start_time = this->current_time;
            this->current_frame_time = 0;
            f = (float)this->len_Frames * 1000.0f / ( this->fps * this->speed );
            this->end_time = this->start_time + (uint64_t)f;
            this->start_frame_time = this->current_frame_time;
            this->end_frame_time = this->current_frame_time + frame_time;
        }
        
        //find end frame
        frm = a->findFrameAtTime( m, this->end_frame_time, &this->end_frame_time );
        if( !frm )
            frm = a->findFrameBeforeTime( m, this->end_frame_time, 0 );
        if( frm )
            this->end_frame = frm->getId();
        
        //find start frame
        frm = a->findFrameBeforeTime( m, this->start_frame_time, &this->start_frame_time );
        if( !frm )
            frm = a->findFrameAtTime( m, this->start_frame_time, 0 );
        if( frm )
            this->start_frame = frm->getId();
        else
            this->start_frame = this->old_end_frame;
        
        this->old_end_frame = this->end_frame;
    }
    
    //return end frame time
    uint64_t model_instance_animation::getEndTime( void )
    {
        float f;
        f = (float)this->end_frame_time * 1000.0f / ( this->fps * this->speed );
        return this->start_time + (uint64_t)f;
    }
    
    //return start frame time
    uint64_t model_instance_animation::getStartTime( void )
    {
        float f;
        f = (float)this->start_frame_time * 1000.0f / ( this->fps * this->speed );
        return this->start_time + (uint64_t)f;
    }
  
    //return animation id
    dpid model_instance_animation::getAnimationId( void )
    {
        return this->anim_id;
    }
    
    //set speed
    void model_instance_animation::setSpeed( float f )
    {
        this->speed = f;
    }
    
    //clone animation
    void model_instance_animation::clone( model_instance_animation *a )
    {
        this->bIsRepeat = a->bIsRepeat;
        this->bIsPlay = a->bIsPlay;
        this->bIsAutplay = a->bIsAutplay;
        this->bDoPlay = a->bDoPlay;
        this->speed = a->speed;
        this->start_time = a->start_time;
        this->end_time = a->end_time;
        this->current_time = a->current_time;
        this->current_frame_time = a->current_frame_time;
        this->start_frame_time = a->start_frame_time;
        this->end_frame_time = a->end_frame_time;
    }
    
};
