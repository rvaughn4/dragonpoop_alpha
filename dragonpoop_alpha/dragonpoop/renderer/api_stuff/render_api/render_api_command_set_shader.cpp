
#include "render_api_command_set_shader.h"
#include "render_api_shader.h"
#include "render_api_shader_ref.h"
#include "render_api_shader_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_shader::render_api_command_set_shader( render_api_commandlist *l, render_api_shader *b ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_shader_writelock *bl;
        
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_shader_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_shader::render_api_command_set_shader" );
        if( !bl )
            return;
        
        this->r = (render_api_shader_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_shader::render_api_command_set_shader( render_api_commandlist *l, render_api_shader_ref *b ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_shader_writelock *bl;
        
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_shader_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_shader::render_api_command_set_shader" );
        if( !bl )
            return;
        
        this->r = (render_api_shader_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_shader::render_api_command_set_shader( render_api_commandlist *l, render_api_shader_writelock *b ) : render_api_command( l )
    {
        if( b )
            this->r = (render_api_shader_ref *)b->getRef();
        else
            this->r = 0;
    }
    
    //dtor
    render_api_command_set_shader::~render_api_command_set_shader( void )
    {
        delete this->r;
    }
    
    //compile command
    void render_api_command_set_shader::compile( render_api_context_writelock *ctx )
    {
        this->setShader( this->r );
    }
    
};
