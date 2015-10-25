
#include "render_api_context_writelock.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    //ctor
    render_api_context_writelock::render_api_context_writelock( render_api_context *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_context_writelock::~render_api_context_writelock( void )
    {
        
    }
  
    //make commandlist
    render_api_commandlist_ref *render_api_context_writelock::makeCmdList( void )
    {
        return this->t->makeCmdList( this );
    }
    
    //delete commandlists
    void render_api_context_writelock::deleteCmdLists( void )
    {
        this->t->deleteCmdLists();
    }
    
    //clear screen
    void render_api_context_writelock::clearColor( float r, float g, float b )
    {
        this->t->clearColor( r, g, b );
    }
    
    //clear depth
    void render_api_context_writelock::clearDepth( float f )
    {
        this->t->clearDepth( f );
    }
    
    //present backbuffer
    void render_api_context_writelock::flipBackBuffer( void )
    {
        this->t->flipBackBuffer();
    }
    
    //make context active in thread
    void render_api_context_writelock::makeActive( void )
    {
        this->t->makeActive();
    }
    
    //set viewport
    void render_api_context_writelock::setViewport( float w, float h )
    {
        this->t->setViewport( w, h );
    }
    
    //make a texture
    render_api_texture_ref *render_api_context_writelock::makeTexture( dpbitmap *bm )
    {
        return this->t->makeTexture( this, bm );
    }
    
    //make a shader
    render_api_shader_ref *render_api_context_writelock::makeShader( const char *cname )
    {
        return this->t->makeShader( this, cname );
    }
    
    //make a vertex buffer
    render_api_vertexbuffer_ref *render_api_context_writelock::makeVertexBuffer( dpvertex_buffer *vb )
    {
        return this->t->makeVertexBuffer( this, vb );
    }
    
    //make a index buffer
    render_api_indexbuffer_ref *render_api_context_writelock::makeIndexBuffer( dpindex_buffer *ib )
    {
        return this->t->makeIndexBuffer( this, ib );
    }
    
};
