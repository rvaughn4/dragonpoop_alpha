
#ifndef dragonpoop_opengl1o5_x11_h
#define dragonpoop_opengl1o5_x11_h

#include "../x11_window/x11_window.h"
#include "../render_api/render_api.h"

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define opengl1o5_x11_max_contexts 4

namespace dragonpoop
{
    
    class opengl1o5_x11 : public render_api
    {
        
    private:
        
        x11_window *w;
        Display *dpy;
        int screen;
        Window win;
        GLXContext ctx, shared_ctx[ opengl1o5_x11_max_contexts ];
        int x, y, ctx_used;
        unsigned int width, height;
        unsigned int depth;
        bool bIsOpen;
        
        //return next fee context
        GLXContext getNextCtx( void );

    protected:
        
        //generate context
        virtual render_api_context *genContext( render_api_writelock *al, dpmutex_master *mm );
        //gen shader
        virtual render_api_shader *genShader( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, const char *cname );
        //gen texture
        virtual render_api_texture *genTexture( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm );
        //gen vertex buffer
        virtual render_api_vertexbuffer *genVertexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb );
        //gen index buffer
        virtual render_api_indexbuffer *genIndexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *vb );

    public:
        
        
        //ctor
        opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        
    };
    
};

#endif