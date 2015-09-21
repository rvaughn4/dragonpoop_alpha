
#include "model_animation.h"
#include "../model_readlock.h"
#include "../model_writelock.h"
#include "../../../core/dpbuffer/dpbuffer.h"
#include "../model_animation_frame/model_animation_frame.h"
#include "../model_frame/model_frame.h"

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
        
        this->bIsAutplay = 0;//h.bIsAutplay;
        this->bIsRepeat = 0;//h.bIsRepeat;
        this->cnt_frames = h.cnt_frames;
        this->len_frames = h.len_frames;
        this->speed = h.speed;
        this->repeat_delay_f = h.repeat_delay_f;
        
        return 1;
    }

    //reduce frame resolution by eliminating frames
    void model_animation::reduceFrames( model_writelock *ml, unsigned int ms_res )
    {
        float frame_step;
        unsigned int s_frm, e_frm;
        model_frame *f;
        bool b;
        
        if( !ms_res )
            return;
        frame_step = (float)ms_res * this->speed / 1000.0f;
        if( frame_step < 2.0f )
            return;
        
        b = 1;
        s_frm = 0;
        e_frm = frame_step;
        while( b )
        {
            f = this->findBiggestFrame( ml, s_frm, e_frm );
            if( !f )
                b = 0;
            else
                this->killSmallerFrames( ml, f, s_frm, e_frm );
            
            s_frm += frame_step;
            e_frm += frame_step;
        }
    }
    
    //return closest frame after time
    model_frame *model_animation::findFrameAtTime( model_writelock *ml, unsigned int t, unsigned int *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
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
        
        if( p_time )
            *p_time = f->getTime();
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }
    
    //return closest frame before time
    model_frame *model_animation::findFrameBeforeTime( model_writelock *ml, unsigned int t, unsigned int *p_time )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
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
        
        if( p_time )
            *p_time = f->getTime();
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }
    
    //returns frame with largest movement within a frame time range
    model_frame *model_animation::findBiggestFrame( model_writelock *ml, unsigned int t_start, unsigned int t_end )
    {
        float td, ad;
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        model_frame *frm;
        
        ml->getAnimationFrames( &l, this->getId() );
        
        f = 0;
        td = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            if( p->getTime() > t_end || p->getTime() < t_start )
                continue;
            
            frm = ml->findFrame( p->getFrameId() );
            if( !frm )
                continue;
            ad = frm->getWeight();
            
            if( ad < td && f )
                continue;
            
            f = p;
            td = ad;
        }
        
        if( !f )
            return 0;
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }
    
    //eliminate all frames smaller than given weight
    void model_animation::killSmallerFrames( model_writelock *ml, model_frame *f, unsigned int t_start, unsigned int t_end )
    {
        float w, pw;
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p;
        model_frame *frm;
        
        if( !f )
            return;
        w = f->getWeight();

        ml->getAnimationFrames( &l, this->getId() );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            if( p->getTime() > t_end || p->getTime() < t_start )
                continue;
            
            frm = ml->findFrame( p->getFrameId() );
            if( !frm || frm == f )
                continue;
            pw = frm->getWeight();
            
            if( pw <= w )
                p->kill();
        }
    }
    
};
