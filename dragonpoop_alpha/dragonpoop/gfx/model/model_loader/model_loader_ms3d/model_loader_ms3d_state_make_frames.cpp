
#include "model_loader_ms3d_state_make_frames.h"
#include "model_loader_ms3d_state_make_triangles.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_frame/model_frame.h"
#include "../../model_frame_joint/model_frame_joint.h"
#include "../../model_animation_frame/model_animation_frame.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include <sstream>

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_frames::model_loader_ms3d_state_make_frames( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_frames::~model_loader_ms3d_state_make_frames( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_frames::run( dpthread_lock *thd, model_loader_writelock *ml )
    {

        this->findUnique( ml );
        this->makeFrames( thd, ml );
        this->makeAnimationFrames( thd, ml );
        this->makeFrameJoints( thd, ml );
        
        return new model_loader_ms3d_state_make_triangles( this->b, this->m );
    }
    
    //find unqiue frame times
    void model_loader_ms3d_state_make_frames::findUnique( model_loader_writelock *ml )
    {
        model_loader_ms3d *ldr;
        std::vector<ms3d_model_joint_m> *l;
        ms3d_model_joint_m *p;
        unsigned int i, e;
        
        ldr = (model_loader_ms3d *)ml->getLoader();
        l = ldr->joints;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            p = &( *l )[ i ];
            this->findUnique( ml, &p->rotate_frames );
            this->findUnique( ml, &p->translate_frames );
        }
    }
    
    //find unique frame times in a joint
    void model_loader_ms3d_state_make_frames::findUnique( model_loader_writelock *ml, std::vector<ms3d_model_joint_keyframe> *l )
    {
        ms3d_model_joint_keyframe *k;
        model_loader_ms3d *ldr;
        unsigned int i, e, ii, ee;
        bool f;
        float fft;
        
        ldr = (model_loader_ms3d *)ml->getLoader();
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            k = &( *l )[ i ];
            fft = k->time * ldr->anim.fps;
            ee = (unsigned int)this->frame_times.size();
            for( f = 0, ii = 0; !f && ii < ee; ii++ )
            {
               if( fft == this->frame_times[ ii ] )
                   f = 1;
            }
            if( !f )
                this->frame_times.push_back( fft );
        }
    }
  
    //make frames
    void model_loader_ms3d_state_make_frames::makeFrames( dpthread_lock *thd, model_loader_writelock *ml )
    {
        model_loader_ms3d *ldr;
        unsigned int i, e;
        ms3d_model_frame f;
        model_frame *frm;
        model_writelock *m;
        shared_obj_guard o;
        std::string s;
        std::vector<ms3d_model_frame> *fl;
        
        ldr = (model_loader_ms3d *)ml->getLoader();
        e = (unsigned int)this->frame_times.size();
        
        fl = ldr->frames;
        if( fl )
            delete fl;
        fl = new std::vector<ms3d_model_frame>();
        ldr->frames = fl;
        
        m = (model_writelock *)o.tryWriteLock( this->m, 1000 );
        if( !m )
            return;
        
        for( i = 0; i < e; i++ )
        {
            std::stringstream ss;
            f.t = this->frame_times[ i ];
            
            f.ot = f.t;
            f.t = f.t * 1000.0f / ldr->anim.fps;
            ss << "MS3D Frame #" << f.ot << "";
            s = ss.str();
            
            frm = m->makeFrame( thd->genId() );
            frm->setName( &s );
            f.id = frm->getId();
            
            fl->push_back( f );
        }
    }
    
    //make animation frames
    void model_loader_ms3d_state_make_frames::makeAnimationFrames( dpthread_lock *thd, model_loader_writelock *ml )
    {
        model_loader_ms3d *ldr;
        unsigned int i, e;
        ms3d_model_frame *f;
        model_animation_frame *frm;
        model_writelock *m;
        shared_obj_guard o;
        std::vector<ms3d_model_frame> *fl;
        
        m = (model_writelock *)o.tryWriteLock( this->m, 1000 );
        if( !m )
            return;
        ldr = (model_loader_ms3d *)ml->getLoader();
        fl = ldr->frames;
        e = (unsigned int)fl->size();
        
        for( i = 0; i < e; i++ )
        {
            f = &( *ldr->frames )[ i ];
            frm = m->makeAnimationFrame( thd->genId(), ldr->anim_id, f->id, f->t );
            f->afid = frm->getId();
        }
    }
    
    //make frame joints
    void model_loader_ms3d_state_make_frames::makeFrameJoints( dpthread_lock *thd, model_loader_writelock *ml )
    {
        model_loader_ms3d *ldr;
        unsigned int i_f, e_f, i_j, e_j;
        model_writelock *m;
        shared_obj_guard o;
        std::vector<ms3d_model_frame> *fl;
        std::vector<ms3d_model_joint_m> *jl;
        ms3d_model_frame *f;
        ms3d_model_joint_m *j;
        model_frame_joint *fjnt;
        dpxyz_f x, ax;
        model_vector v;
        model_quaternion q;
        model_matrix mx;
        
        m = (model_writelock *)o.tryWriteLock( this->m, 1000 );
        if( !m )
            return;
        ldr = (model_loader_ms3d *)ml->getLoader();
        fl = ldr->frames;
        jl = ldr->joints;
        e_f = (unsigned int)fl->size();
        e_j = (unsigned int)jl->size();
        
        for( i_f = 0; i_f < e_f; i_f++ )
        {
            f = &( *fl )[ i_f ];
            
            for( i_j = 0; i_j < e_j; i_j++ )
            {
                j = &( *jl )[ i_j ];
                
                fjnt = m->makeFrameJoint( thd->genId(), f->id, j->id );
                
                this->getKeyframe( f->ot, &j->translate_frames, &v );
                v.getPosition( &x );
                fjnt->setTranslation( &x );
//
  //              this->getKeyframeQuat( f->ot, &j->rotate_frames, &q );
    //            mx.setQuat( &q );
      //          mx.setPosition( &v );
        //        mx.getAngles( &ax );
                
                this->getKeyframe( f->ot, &j->rotate_frames, &v );
                v.getPosition( &ax );
                fjnt->setRotation( &ax );
            }
        }
        
    }
    
    //find xyz keyframe before and after time and interpolate
    void model_loader_ms3d_state_make_frames::getKeyframe( float t, std::vector<ms3d_model_joint_keyframe> *l, model_vector *x )
    {
        ms3d_model_joint_keyframe kb, ke;
        float td, tt, rb, re;
        dpxyz_f y;
        
        this->getKeyframeBefore( t, l, &kb );
        this->getKeyframeAfter( t, l, &ke );
        
        td = ke.time - kb.time;
        tt = t - kb.time;
        if( td <= 0 )
            re = 0;
        else
            re = tt / td;
        if( re > 1 )
            re = 1;
        rb = 1.0f - re;
        
        y.x = rb * kb.x + re * ke.x;
        y.y = rb * kb.y + re * ke.y;
        y.z = rb * kb.z + re * ke.z;
        
        x->setPosition( &y );
    }

    //find xyz keyframe before and after time and interpolate
    void model_loader_ms3d_state_make_frames::getKeyframeQuat( float t, std::vector<ms3d_model_joint_keyframe> *l, model_quaternion *x )
    {
        ms3d_model_joint_keyframe kb, ke;
        float td, tt, rb, re;
        model_quaternion qb, qe;
        
        this->getKeyframeBefore( t, l, &kb );
        this->getKeyframeAfter( t, l, &ke );
        
        td = ke.time - kb.time;
        tt = t - kb.time;
        if( td <= 0 )
            re = 0;
        else
            re = tt / td;
        if( re > 1 )
            re = 1;
        re = 1;
        rb = 1.0f - re;
        
        qb.setAngle( kb.x, kb.y, kb.z );
        qe.setAngle( ke.x, ke.y, ke.z );
        
        x->slerp( &qb, &qe, t );
    }
    
    //find xyz keyframe before time
    void model_loader_ms3d_state_make_frames::getKeyframeBefore( float t, std::vector<ms3d_model_joint_keyframe> *l, ms3d_model_joint_keyframe *x )
    {
        unsigned int i, e;
        bool f;
        ms3d_model_joint_keyframe *p;
        
        x->time = 0;
        f = 0;
        
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            p = &( *l )[ i ];
            
            if( p->time > t )
                continue;
            if( p->time >= x->time )
            {
                *x = *p;
                f = 1;
            }
        }
    
        if( !f )
        {
            x->time = t;
            x->x = 0;
            x->y = 0;
            x->z = 0;
        }
    }
    
    //find xyz keyframe after time
    void model_loader_ms3d_state_make_frames::getKeyframeAfter( float t, std::vector<ms3d_model_joint_keyframe> *l, ms3d_model_joint_keyframe *x )
    {
        unsigned int i, e;
        bool f;
        ms3d_model_joint_keyframe *p;
        
        x->time = 0;
        f = 0;
        
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            p = &( *l )[ i ];
            
            if( p->time < t )
                continue;
            if( p->time <= x->time )
            {
                *x = *p;
                f = 1;
            }
        }
        
        if( !f )
            this->getKeyframeBefore( t, l, x );
    }
    
};
