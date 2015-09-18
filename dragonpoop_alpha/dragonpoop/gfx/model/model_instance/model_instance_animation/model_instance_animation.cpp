
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
        this->anim_id = m->getId();
        this->bIsRepeat = m->isRepeated();
        this->bIsAutplay = m->isAutoPlay();
        this->repeat_delay_f = m->getRepeatDelay();
        this->len_Frames = m->getLength();
        this->cnt_frames = m->getFrameCount();
        this->fps = m->getFps();
        this->bIsPlay = 0;
        this->bDoPlay = this->bIsAutplay;
        this->start_time = this->end_time = 0;
        this->end_frame_time = this->start_frame_time = 0;
        dpid_zero( &this->start_frame );
        dpid_zero( &this->end_frame );
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
    model_frame *model_instance_animation::findFrameAtTime( model_writelock *ml, unsigned int t, unsigned int *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        ml->getAnimationFrames( &l, this->getAnimationId() );
        
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
        
        if( p_time )
            *p_time = f->getTime();
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }

    //return closest frame before time
    model_frame *model_instance_animation::findFrameBeforeTime( model_writelock *ml, unsigned int t, unsigned int *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        ml->getAnimationFrames( &l, this->getAnimationId() );
        
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
        
        if( p_time )
            *p_time = f->getTime();
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }

    //returns frame with largest movement within a frame time range
    model_frame *model_instance_animation::findBiggestFrame( model_writelock *ml, unsigned int t_start, unsigned int t_end )
    {
        float od, td, ad;
        dpxyz_f x;
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        std::list<model_frame_joint *> lj;
        std::list<model_frame_joint *>::iterator ij;
        model_frame_joint *pj;
        
        ml->getAnimationFrames( &l, this->getAnimationId() );
        
        f = 0;
        td = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;

            if( p->getTime() > t_end || p->getTime() < t_start )
                continue;
            
            ml->getFrameJoints( &lj, p->getFrameId() );
            ad = 0;
            
            for( ij = lj.begin(); ij != lj.end(); ++ij )
            {
                pj = *ij;
                
                pj->getRotation( &x );
                od = x.x * x.x + x.y * x.y + x.z * x.z;
                if( od > 0 )
                    od = sqrtf( od );
                ad += od * 2;

                pj->getTranslation( &x );
                od = x.x * x.x + x.y * x.y + x.z * x.z;
                if( od > 0 )
                    od = sqrtf( od );
                ad += od / 10.0f;
            }
            
            if( ad < td && f )
                continue;
            
            f = p;
            td = ad;
        }
        
        if( !f )
            return 0;
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
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
        unsigned int old_time;
        
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
            if( this->bIsRepeat )
            {
                this->start_frame_time = this->end_frame_time;
                this->start_frame = this->end_frame;
            }
            else
            {
                dpid_zero( &this->start_frame );
                this->start_frame_time = 0;
            }
            if( !this->bDoPlay )
                return;
        }
        
        //start playing
        if( !this->bIsPlay )
        {
            this->start_time = this->current_time;
            this->current_frame_time = 0;
            f = (float)this->len_Frames * 1000.0f / this->fps;
            this->end_time = this->start_time + (uint64_t)f;
            this->bIsPlay = 1;
        }
        
        //calc current animation frame
        f = (float)this->current_time - (float)this->start_time;
        f = f *this->fps / 1000.0f;
        old_time = this->current_frame_time;
        this->current_frame_time = (unsigned int)f;
        if( old_time > this->current_frame_time )
            old_time = this->current_frame_time;
        
        //find start frame
        frm = this->findFrameBeforeTime( m, this->current_frame_time, &this->start_frame_time );
        if( frm )
            this->start_frame = frm->getId();
        
        //find end frame
        frm = this->findFrameAtTime( m, this->current_frame_time + frame_time, &this->end_frame_time );
        if( frm )
            this->end_frame = frm->getId();
        
        //find biggest frame
        frm = this->findBiggestFrame( m, old_time, this->end_frame_time );
        if( frm )
            this->end_frame = frm->getId();
    }
    
    //return end frame time
    uint64_t model_instance_animation::getEndTime( void )
    {
        float f;
        f = (float)this->end_frame_time * 1000.0f / this->fps;
        return this->start_time + (uint64_t)f;
    }
    
    //return start frame time
    uint64_t model_instance_animation::getStartTime( void )
    {
        float f;
        f = (float)this->start_frame_time * 1000.0f / this->fps;
        return this->start_time + (uint64_t)f;
    }
  
    //return animation id
    dpid model_instance_animation::getAnimationId( void )
    {
        return this->anim_id;
    }
    
};
