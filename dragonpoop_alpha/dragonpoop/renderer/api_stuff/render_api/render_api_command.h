
#ifndef dragonpoop_render_api_command_h
#define dragonpoop_render_api_command_h

namespace dragonpoop
{
    
    class render_api_commandlist;
    class render_api_context_writelock;
    class render_api_shader_ref;
    class render_api_texture_ref;
    class render_api_vertexbuffer_ref;
    class render_api_indexbuffer_ref;
    class dpmatrix;
    
    class render_api_command
    {
        
    private:
        
        render_api_commandlist *l;
        
    protected:
        
        //set matrix
        void setMatrix( dpmatrix *m );
        //set current shader
        void setShader( render_api_shader_ref *r );
        //set current texture 0
        void setTexture( render_api_texture_ref *r, int index );
        //set current vertex buffer
        void setVertexBuffer( render_api_vertexbuffer_ref *r );
        //set current index buffer
        void setIndexBuffer( render_api_indexbuffer_ref *r );
        //set alpha
        void setAlpha( float a );
        //set color
        void setColor( float r, float g, float b );
        //draw
        void draw( render_api_context_writelock *ctx );
        
    public:
        
        //ctor
        render_api_command( render_api_commandlist *l );
        //dtor
        virtual ~render_api_command( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );

    };
    
};

#endif