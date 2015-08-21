
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
#include "../../model_animation_frame/model_animation_frame.h"
#include "../../../../core/dpthread/dpthread_lock.h"

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
    
};
