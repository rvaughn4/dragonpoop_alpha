
#include "model_saver_ms3d_state_write_joints.h"
#include "model_saver_ms3d_state_write_animation.h"
#include "model_saver_ms3d_joint.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include <vector>

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_joints::model_saver_ms3d_state_write_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_joints::~model_saver_ms3d_state_write_joints( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_joints::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_joint_section h;
        ms3d_model_joint_m *v;
        std::vector<ms3d_model_joint_m> *l;
        unsigned int i, e, j, k;
        model_saver_ms3d *t;
        
        t = (model_saver_ms3d *)ml->getSaver();
        l = t->joints;
        if( !l )
            e = 0;
        else
            e = (unsigned int)l->size();
        
        h.cnt = e;
        this->b->writeBytes( (uint8_t *)&h, sizeof( h ) );
        
        for( i = 0; i < e; i++ )
        {
            v = &( *l )[ i ];
            
            v->f.cnt_rot_frames = (unsigned int)v->rotate_frames.size();
            v->f.cnt_pos_frames = (unsigned int)v->translate_frames.size();
            
            this->b->writeBytes( (uint8_t *)&v->f, sizeof( v->f ) );
            
            k = v->f.cnt_rot_frames;
            for( j = 0; j < k; j++ )
                this->b->writeBytes( (uint8_t *)&v->rotate_frames[ j ], sizeof( v->rotate_frames[ 0 ] ) );
            
            k = v->f.cnt_pos_frames;
            for( j = 0; j < k; j++ )
                this->b->writeBytes( (uint8_t *)&v->translate_frames[ j ], sizeof( v->translate_frames[ 0 ] ) );
        }
        
        return new model_saver_ms3d_state_cleanup( this->b, this->m, 1 );
    }
    
};
