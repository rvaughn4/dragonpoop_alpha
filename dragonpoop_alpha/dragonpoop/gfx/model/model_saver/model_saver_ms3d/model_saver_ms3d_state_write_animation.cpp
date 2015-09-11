
#include "model_saver_ms3d_state_write_animation.h"
#include "model_saver_ms3d_state_write_joints.h"
#include "model_saver_ms3d_material.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include <vector>

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_animation::model_saver_ms3d_state_write_animation( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_animation::~model_saver_ms3d_state_write_animation( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_animation::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_animation_section h;
        model_saver_ms3d *t;
        
        t = (model_saver_ms3d *)ml->getSaver();
        
        h = t->anim;
        this->b->writeBytes( (uint8_t *)&h, sizeof( h ) );
        
        return new model_saver_ms3d_state_write_joints( this->b, this->m );
    }
    
};
