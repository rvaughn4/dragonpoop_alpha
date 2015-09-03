
#include "model_loader_ms3d_state_parse_animation.h"
#include "model_loader_ms3d_state_parse_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_animation.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_animation::model_loader_ms3d_state_parse_animation( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_parse_animation::~model_loader_ms3d_state_parse_animation( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_animation::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        
        ms3d_model_animation_section sh;
        model_loader_ms3d *m;
        
        m = (model_loader_ms3d *)ml->getLoader();
        
        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        m->anim = sh;
        m->anim.fps = 5;
        
        return new model_loader_ms3d_state_parse_joints( this->b, this->m );
    }
    
};
