
#ifndef dragonpoop_opengl1o5_x11_context_h
#define dragonpoop_opengl1o5_x11_context_h

#include "opengl1o5_x11.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_context : public render_api_context
    {
        
    private:
        
        GLXContext ctx;
        Window win;
        Display *dpy;
        
    protected:
        
        //generate commandlist
        virtual render_api_commandlist *genCmdList( render_api_context_writelock *cl, dpmutex_master *mm );
        //clear screen
        virtual void clearColor( float r, float g, float b );
        //clear depth
        virtual void clearDepth( float f );
        //present backbuffer
        virtual void flipBackBuffer( void );
        //make context active in thread
        virtual void makeActive( void );
        //set viewport
        virtual void setViewport( float w, float h );
        //gen shader
        virtual render_api_shader *genShader( dpmutex_master *mm, render_api_context_writelock *cl, const char *cname );
        //gen texture
        virtual render_api_texture *genTexture( dpmutex_master *mm, render_api_context_writelock *cl, dpbitmap *bm );
        //gen vertex buffer
        virtual render_api_vertexbuffer *genVertexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpvertex_buffer *vb );
        //gen index buffer
        virtual render_api_indexbuffer *genIndexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpindex_buffer *vb );

    public:
        
        //ctor
        opengl1o5_x11_context( render_api_writelock *r, dpmutex_master *mm, GLXContext ctx, Window win, Display *dpy );
        //dtor
        virtual ~opengl1o5_x11_context( void );
        
    };
    
};

#endif