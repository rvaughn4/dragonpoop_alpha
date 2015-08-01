
#include "model_loader_ms3d_state_parse_header.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_header.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_state_sucess.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_parse_header::model_loader_ms3d_state_parse_header( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_loader_ms3d_state_parse_header::~model_loader_ms3d_state_parse_header( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_header::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        ms3d_model_header h;
        std::string s;
        
        if( this->b->readBytes( (uint8_t *)&h, sizeof( h ) ) < sizeof( h ) )
            return new model_loader_state_fail( this->b );
        
        if( h.version != 4 )
            return new model_loader_state_fail( this->b );
        
        h.version = 0;
        s.assign( (const char *)h.ms3d_magic );
        if( s.compare( ms3d_magic_number ) != 0 )
            return new model_loader_state_fail( this->b );
        
        return new model_loader_state_sucess( this->b );
    }

};
