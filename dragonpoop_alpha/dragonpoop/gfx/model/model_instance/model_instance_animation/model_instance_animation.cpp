
#include "model_instance_animation.h"
#include "../../model_animation/model_animation.h"
#include "../../model_readlock.h"
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
    void model_instance_animation::start( dpthread_lock *thd, model_readlock *ml )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p;
        
        this->start_time = thd->getTicks();
        this->end_time = 0;
        
        ml->getComponentsByParent( model_component_type_animation_frame, this->getId(), (std::list<model_component *> *)&l );
        
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
    }
    
    //returns true if playing
    bool model_instance_animation::isPlaying( void )
    {
        return this->start_time != 0;
    }
    
    //return ms played
    uint64_t model_instance_animation::getPlayTime( dpthread_lock *thd )
    {
        uint64_t t, td;
        
        if( !this->start_time || !this->end_time )
            return 0;
            
        t = thd->getTicks();
        if( t > this->start_time )
            td = t - this->start_time;
        else
            td = 0;
        
        if( td > this->end_time )
        {
            this->start_time = t + this->repeat_delay_ms - ( td - this->end_time );
            if( t > this->start_time )
                td = t - this->start_time;
            else
                td = 0;
        }
        
        return td;
    }
    
    //return closest frame after time
    model_frame *model_instance_animation::getFrameAtTime( model_readlock *ml, uint64_t t )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p, *f;
        
        ml->getComponentsByParent( model_component_type_animation_frame, this->getId(), (std::list<model_component *> *)&l );
        
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
        
        return (model_frame *)ml->findComponent( model_component_type_frame, f->getFrameId() );
    }
    
};
