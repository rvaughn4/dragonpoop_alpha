
#include "model_saver_ms3d_state_make_animation.h"
#include "model_saver_ms3d_state_make_frames.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_animation.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_animation/model_animation.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../model_animation_frame/model_animation_frame.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_animation::model_saver_ms3d_state_make_animation( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_animation::~model_saver_ms3d_state_make_animation( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_animation::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_animation *ma;
        model_saver_ms3d *t;
        std::list<model_animation *> l;
        std::list<model_animation *>::iterator i;
        unsigned int start_frame;
        float c;
        std::vector<ms3d_model_animation_m> *la;
        
        m = (model_readlock *)o.readLock( this->m );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        la = t->anims;
        if( la )
            delete la;
        la = new std::vector<ms3d_model_animation_m>();
        t->anims = la;
        t->anim.fps = 30.0f;

        m->getAnimations( &l );
        start_frame = 0;
        c = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            ma = *i;
            c += this->makeAnimation( ma, t, m, &start_frame );
        }
        
        t->anim.fps = 30.0f;
        t->anim.current_time = 0;
        t->anim.cnt_frames = c;
        
        return new model_saver_ms3d_state_make_frames( this->b, this->m );
    }
    
    float model_saver_ms3d_state_make_animation::makeAnimation( model_animation *a, model_saver_ms3d *t, model_readlock *m, unsigned int *start_frame )
    {
        unsigned int highest_frame, tm, c;
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p;
        ms3d_model_animation_m s;
        std::vector<ms3d_model_animation_m> *la;
        float ftm, fhighest_frame;

        m->getAnimationFrames( &l, a->getId() );
        c = highest_frame = 0;
        fhighest_frame = 0;
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            tm = p->getTime();
            if( tm > highest_frame )
                highest_frame = tm;
            ftm = tm * t->anim.fps / 1000.0f;
            if( ftm > fhighest_frame )
                fhighest_frame = ftm;
            c++;
        }
        
        s.id = a->getId();
        s.cnt_frames = c;
        s.length = highest_frame;
        s.highest_time = fhighest_frame;
        if( c )
            s.skip = highest_frame / c;
        else
            s.skip = 1;
        fhighest_frame += (float)s.skip * t->anim.fps / 1000.0f;
        
        s.start = *start_frame;
        *start_frame += highest_frame + s.skip;
        
        la = t->anims;
        la->push_back( s );
        
        return fhighest_frame;
    }
    
};
