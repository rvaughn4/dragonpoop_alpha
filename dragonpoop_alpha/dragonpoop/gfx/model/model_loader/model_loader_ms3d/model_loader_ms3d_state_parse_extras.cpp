
#include "model_loader_ms3d_state_parse_extras.h"
#include "model_loader_ms3d_state_parse_comments.h"
#include "model_loader_ms3d_state_make_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d_comment.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_extras::model_loader_ms3d_state_parse_extras( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_parse_extras::~model_loader_ms3d_state_parse_extras( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_extras::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        
        ms3d_model_extras_section sh;
        
        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        if( sh.version != 1 )
            return new model_loader_ms3d_state_make_joints( this->b, this->m );
        
        return new model_loader_ms3d_state_parse_comments( this->b, this->m );
    }
    
};
