
#include "model_loader_ms3d_state_make_frames.h"
#include "model_loader_ms3d_state_make_verts.h"
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

#include <iostream>
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
        
        return new model_loader_ms3d_state_make_verts( this->b, this->m );
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
        int ft;
        float fft;
        
        ldr = (model_loader_ms3d *)ml->getLoader();
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            k = &( *l )[ i ];
            fft = k->time * ldr->anim.fps;
            ft = fft;
            ee = (unsigned int)this->frame_times.size();
            for( f = 0, ii = 0; !f && ii < ee; ii++ )
            {
               if( ft == this->frame_times[ ii ] )
                   f = 1;
            }
            if( !f )
                this->frame_times.push_back( ft );
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
            
            ss << "MS3D Frame #" << f.t << "";
            s = ss.str();
            
            f.ot = (float)f.t / ldr->anim.fps;
            f.t = f.t * 1000 / (int)ldr->anim.fps;
            
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
        dpxyzw x;
        
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
            
            //std::cout << "frame " << i_f << " " << f->t << " " << f->ot << "\r\n";
            
            for( i_j = 0; i_j < e_f; i_j++ )
            {
                j = &( *jl )[ i_j ];
                
                fjnt = m->makeFrameJoint( thd->genId(), f->id, j->id );
                
                this->getKeyframe( f->t, &j->rotate_frames, &x );
                fjnt->setRotation( &x );
                this->getKeyframe( f->t, &j->translate_frames, &x );
                fjnt->setTranslation( &x );
                
                //std::cout << "\tjoint " << i_j << "\r\n";
            }
        }
        
        
        
    }
    
    //find xyz keyframe before and after time and interpolate
    void model_loader_ms3d_state_make_frames::getKeyframe( float t, std::vector<ms3d_model_joint_keyframe> *l, dpxyzw *x )
    {
        
    }
    
};
