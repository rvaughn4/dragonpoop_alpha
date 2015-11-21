
#ifndef dragonpoop_opengl1o5_x11_h
#define dragonpoop_opengl1o5_x11_h

#include "../x11_window/x11_window.h"
#include "../render_api/render_api.h"

#include <dlfcn.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define opengl1o5_x11_max_contexts 10

namespace dragonpoop
{

    typedef int opengl1o5_x11_GLint;
    typedef unsigned int opengl1o5_x11_GLuint;
    typedef int opengl1o5_x11_GLenum;
    typedef int opengl1o5_x11_GLsizei;
    typedef void opengl1o5_x11_GLvoid;

    typedef void (* opengl1o5_x11_functions_glEnableClientState )( opengl1o5_x11_GLint state );
    typedef void (* opengl1o5_x11_functions_glTexCoordPointer )( opengl1o5_x11_GLint size, opengl1o5_x11_GLenum type, opengl1o5_x11_GLsizei stride, const opengl1o5_x11_GLvoid *ptr );
    typedef void (* opengl1o5_x11_functions_glNormalPointer )( opengl1o5_x11_GLenum type, opengl1o5_x11_GLsizei stride, const opengl1o5_x11_GLvoid *ptr );
    typedef void (* opengl1o5_x11_functions_glVertexPointer )( opengl1o5_x11_GLint size, opengl1o5_x11_GLenum type, opengl1o5_x11_GLsizei stride, const opengl1o5_x11_GLvoid *ptr );
    typedef void (* opengl1o5_x11_functions_glDrawElements )( opengl1o5_x11_GLenum mode, opengl1o5_x11_GLsizei count, opengl1o5_x11_GLenum type, const opengl1o5_x11_GLvoid *indicies );
    typedef void (* opengl1o5_x11_functions_glGenTextures )( opengl1o5_x11_GLsizei n, opengl1o5_x11_GLuint *textures );
    typedef void (* opengl1o5_x11_functions_glDeleteTextures )( opengl1o5_x11_GLsizei n, opengl1o5_x11_GLuint *textures );
    typedef void (* opengl1o5_x11_functions_glBindTexture )( opengl1o5_x11_GLenum target, opengl1o5_x11_GLuint texture );
    typedef void (* opengl1o5_x11_functions_glTexImage2D )( opengl1o5_x11_GLenum target, opengl1o5_x11_GLint level, opengl1o5_x11_GLint internal_format, opengl1o5_x11_GLsizei w, opengl1o5_x11_GLsizei h, opengl1o5_x11_GLint border, opengl1o5_x11_GLenum format, opengl1o5_x11_GLenum type, opengl1o5_x11_GLvoid *pixels );
    typedef void (* opengl1o5_x11_functions_glTexParameteri )( opengl1o5_x11_GLenum target, opengl1o5_x11_GLenum pname, opengl1o5_x11_GLint param );

//    typedef void (* opengl1o5_x11_functions_

    struct opengl1o5_x11_functions
    {
        opengl1o5_x11_functions_glEnableClientState glEnableClientState;
        opengl1o5_x11_functions_glTexCoordPointer glTexCoordPointer;
        opengl1o5_x11_functions_glNormalPointer glNormalPointer;
        opengl1o5_x11_functions_glVertexPointer glVertexPointer;
        opengl1o5_x11_functions_glDrawElements glDrawElements;
        opengl1o5_x11_functions_glGenTextures glGenTextures;
        opengl1o5_x11_functions_glDeleteTextures glDeleteTextures;
        opengl1o5_x11_functions_glBindTexture glBindTexture;
        opengl1o5_x11_functions_glTexImage2D glTexImage2D;
        opengl1o5_x11_functions_glTexParameteri glTexParameteri;

/*



       */ /*

        glClearColor( r, g, b, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glClearDepth( f );
        glXSwapBuffers( this->dpy, this->win );
        glViewport( 0, 0, w, h );
*/
    };

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
        bool bIsOpen, bUseDl;
        void *dlGL;

        opengl1o5_x11_functions gl;

        //return next fee context
        GLXContext getNextCtx( void );
        //load functions
        bool loadFunctions( void );

    protected:

        //generate context
        virtual render_api_context *genContext( render_api_writelock *al, dpmutex_master *mm );

    public:


        //ctor
        opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm, bool bUseDl );
        //dtor
        virtual ~opengl1o5_x11( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );

    };

};

#endif
