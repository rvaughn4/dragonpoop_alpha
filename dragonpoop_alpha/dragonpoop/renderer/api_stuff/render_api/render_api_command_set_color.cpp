
#include "render_api_command_set_color.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_color::render_api_command_set_color( render_api_commandlist *l, float r, float g, float b ) : render_api_command( l )
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    
    //dtor
    render_api_command_set_color::~render_api_command_set_color( void )
    {
    }
    
    //compile command
    void render_api_command_set_color::compile( render_api_context_writelock *ctx )
    {
        this->setColor( this->r, this->g, this->b );
    }
    
};
