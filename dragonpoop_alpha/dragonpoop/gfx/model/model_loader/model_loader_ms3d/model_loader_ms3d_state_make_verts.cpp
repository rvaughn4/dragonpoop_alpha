
#include "model_loader_ms3d_state_make_verts.h"
#include "model_loader_ms3d_state_parse_triangles.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_verts::model_loader_ms3d_state_make_verts( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_verts::~model_loader_ms3d_state_make_verts( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_verts::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard om, ov;
        model_writelock *m;
        
        m = (model_writelock *)om.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        
        return new model_loader_ms3d_state_parse_triangles( this->b, this->m );
    }
    
};
