
#include "render_api_command_draw.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_draw::render_api_command_draw( render_api_commandlist *l ) : render_api_command( l )
    {
        
    }
    
    //dtor
    render_api_command_draw::~render_api_command_draw( void )
    {
        
    }
    
    //compile command
    void render_api_command_draw::compile( render_api_context_writelock *ctx )
    {
        this->draw( ctx );
    }

};
