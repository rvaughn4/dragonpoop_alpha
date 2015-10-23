
#include "render_api_command_set_vertexbuffer.h"
#include "render_api_vertexbuffer.h"
#include "render_api_vertexbuffer_ref.h"
#include "render_api_vertexbuffer_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    render_api_command_set_vertexbuffer::render_api_command_set_vertexbuffer( render_api_commandlist *l, render_api_vertexbuffer *b ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_vertexbuffer_writelock *bl;
        
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_vertexbuffer_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_vertexbuffer::render_api_command_set_vertexbuffer" );
        if( !bl )
            return;
        
        this->r = (render_api_vertexbuffer_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_vertexbuffer::render_api_command_set_vertexbuffer( render_api_commandlist *l, render_api_vertexbuffer_ref *b ) : render_api_command( l )
    {
        shared_obj_guard o;
        render_api_vertexbuffer_writelock *bl;
        
        this->r = 0;
        if( !b )
            return;
        
        bl = (render_api_vertexbuffer_writelock *)o.tryWriteLock( b, 1000, "render_api_command_set_vertexbuffer::render_api_command_set_vertexbuffer" );
        if( !bl )
            return;
        
        this->r = (render_api_vertexbuffer_ref *)bl->getRef();
    }
    
    //ctor
    render_api_command_set_vertexbuffer::render_api_command_set_vertexbuffer( render_api_commandlist *l, render_api_vertexbuffer_writelock *b ) : render_api_command( l )
    {
        if( b )
            this->r = (render_api_vertexbuffer_ref *)b->getRef();
        else
            this->r = 0;
    }
    
    //dtor
    render_api_command_set_vertexbuffer::~render_api_command_set_vertexbuffer( void )
    {
        delete this->r;
    }
    
    //compile command
    void render_api_command_set_vertexbuffer::compile( render_api_context_writelock *ctx )
    {
        this->setVertexBuffer( this->r );
    }
    
};
