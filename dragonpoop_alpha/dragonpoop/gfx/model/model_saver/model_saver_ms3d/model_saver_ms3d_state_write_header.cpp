
#include "model_saver_ms3d_state_write_header.h"
#include "model_saver_ms3d_state_write_verts.h"
#include "model_saver_ms3d_header.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_write_header::model_saver_ms3d_state_write_header( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_write_header::~model_saver_ms3d_state_write_header( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_write_header::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        ms3d_model_header h;
        std::string s;
        
        h.version = 4;
        s.assign( ms3d_magic_number );
        s.copy( (char *)h.ms3d_magic, sizeof( h.ms3d_magic ) );
        this->b->writeBytes( (uint8_t *)&h, sizeof( h ) );
        
        return new model_saver_ms3d_state_write_verts( this->b, this->m );
    }

};
