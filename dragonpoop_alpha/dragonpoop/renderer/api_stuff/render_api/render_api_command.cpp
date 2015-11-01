
#include "render_api_command.h"
#include "render_api_commandlist.h"

namespace dragonpoop
{

    //ctor
    render_api_command::render_api_command( render_api_commandlist *l )
    {
        this->l = l;
    }
    
    //dtor
    render_api_command::~render_api_command( void )
    {
        
    }
    
    //compile command
    void render_api_command::compile( render_api_context_writelock *ctx )
    {
        
    }
    
    //set current shader
    void render_api_command::setShader( render_api_shader_ref *r )
    {
        this->l->cmd_setShader( r );
    }
    
    //set current texture 0
    void render_api_command::setTexture( render_api_texture_ref *r, int index )
    {
        this->l->cmd_setTexture( r, index );
    }
    
    //set current vertex buffer
    void render_api_command::setVertexBuffer( render_api_vertexbuffer_ref *r )
    {
        this->l->cmd_setVertexBuffer( r );
    }
    
    //set current index buffer
    void render_api_command::setIndexBuffer( render_api_indexbuffer_ref *r )
    {
        this->l->cmd_setIndexBuffer( r );
    }
    
    //set alpha
    void render_api_command::setAlpha( float a )
    {
        this->l->cmd_setAlpha( a );
    }
    
    //draw
    void render_api_command::draw( render_api_context_writelock *ctx )
    {
        this->l->cmd_draw( ctx );
    }
    
    //set matrix
    void render_api_command::setMatrix( dpmatrix *m )
    {
        this->l->cmd_setMatrix( m );
    }

};
