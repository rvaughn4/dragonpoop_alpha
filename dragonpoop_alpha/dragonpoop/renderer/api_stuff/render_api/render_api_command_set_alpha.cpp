
#include "render_api_command_set_alpha.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_alpha::render_api_command_set_alpha( render_api_commandlist *l, float a ) : render_api_command( l )
    {
        this->a = a;
    }
    
    //dtor
    render_api_command_set_alpha::~render_api_command_set_alpha( void )
    {
    }
    
    //compile command
    void render_api_command_set_alpha::compile( render_api_context_writelock *ctx )
    {
        this->setAlpha( this->a );
    }
    
};
