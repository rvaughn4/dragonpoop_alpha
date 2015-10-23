
#include "render_api_command_set_matrix.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_matrix::render_api_command_set_matrix( render_api_commandlist *l, dpmatrix *m ) : render_api_command( l )
    {
        this->m.copy( m );
    }
    
    //dtor
    render_api_command_set_matrix::~render_api_command_set_matrix( void )
    {
    }
    
    //compile command
    void render_api_command_set_matrix::compile( render_api_context_writelock *ctx )
    {
        this->setMatrix( &this->m );
    }
    
};
