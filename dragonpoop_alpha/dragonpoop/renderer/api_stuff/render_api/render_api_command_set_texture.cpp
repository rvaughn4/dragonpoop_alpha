
#include "render_api_command_set_texture.h"
#include "render_api_texture.h"
#include "render_api_texture_ref.h"
#include "render_api_texture_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_texture::render_api_command_set_texture( render_api_commandlist *l, render_api_texture *b, int tex_index ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_texture_writelock *bl;
        
        this->tex_index = tex_index;
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_texture_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_texture::render_api_command_set_texture" );
        if( !bl )
            return;
        
        this->r = (render_api_texture_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_texture::render_api_command_set_texture( render_api_commandlist *l, render_api_texture_ref *b, int tex_index ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_texture_writelock *bl;
        
        this->tex_index = tex_index;
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_texture_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_texture::render_api_command_set_texture" );
        if( !bl )
            return;
        
        this->r = (render_api_texture_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_texture::render_api_command_set_texture( render_api_commandlist *l, render_api_texture_writelock *b, int tex_index ) : render_api_command( l )
    {
        this->tex_index = tex_index;
        if( b )
            this->r = (render_api_texture_ref *)b->getRef();
        else
            this->r = 0;
    }
    
    //dtor
    render_api_command_set_texture::~render_api_command_set_texture( void )
    {
        delete this->r;
    }
    
    //compile command
    void render_api_command_set_texture::compile( render_api_context_writelock *ctx )
    {
        if( !this->tex_index )
            this->setTexture0( this->r );
        else
            this->setTexture1( this->r );
    }
    
};
