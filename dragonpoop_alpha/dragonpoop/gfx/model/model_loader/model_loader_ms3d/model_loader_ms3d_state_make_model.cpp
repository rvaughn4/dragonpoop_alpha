
#include "model_loader_ms3d_state_make_model.h"
#include "model_loader_ms3d_state_parse_verts.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_header.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_model::model_loader_ms3d_state_make_model( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_ms3d_state_make_model::~model_loader_ms3d_state_make_model( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_model::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        model_ref *mdr;
        
        mdr = ml->getModel();
        
        return new model_loader_ms3d_state_parse_verts( this->b, mdr );
    }
    
};
