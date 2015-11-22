
#ifndef dragonpoop_opengl1o5_x11_h
#define dragonpoop_opengl1o5_x11_h

#include "../x11_window/x11_window.h"
#include "../render_api/render_api.h"

#include <dlfcn.h>
#include <string>

#define opengl1o5_x11_max_contexts 10

namespace dragonpoop
{

    typedef int opengl1o5_x11_GLint;
    typedef unsigned int opengl1o5_x11_GLuint;
    typedef int opengl1o5_x11_GLenum;
    typedef int opengl1o5_x11_GLsizei;
    typedef void opengl1o5_x11_GLvoid;
    typedef unsigned char opengl1o5_x11_GLubyte;
    typedef float opengl1o5_x11_GLfloat;
    typedef double opengl1o5_x11_GLdouble;
    typedef unsigned int opengl1o5_x11_GLbitfield;
    typedef GLXDrawable opengl1o5_x11_GLXDrawable;
    typedef GLXContext opengl1o5_x11_GLXContext;

    typedef void *(* opengl1o5_x11_functions_glXGetProcAddress )( const opengl1o5_x11_GLubyte *procName );
    typedef void (* opengl1o5_x11_functions_glXSwapBuffers )( x11_window_Display *dpy, opengl1o5_x11_GLXDrawable drawable );
    typedef bool (* opengl1o5_x11_functions_glXMakeCurrent )( x11_window_Display *dpy, opengl1o5_x11_GLXDrawable drawable, opengl1o5_x11_GLXContext ctx );
    typedef opengl1o5_x11_GLXContext (* opengl1o5_x11_functions_glXCreateContext )( x11_window_Display *dpy, x11_window_XVisualInfo *vis, opengl1o5_x11_GLXContext shareList, bool direct );
    typedef void (* opengl1o5_x11_functions_glXDestroyContext )( x11_window_Display *dpy, opengl1o5_x11_GLXContext ctx );

    typedef const opengl1o5_x11_GLubyte * (* opengl1o5_x11_functions_glGetString )( opengl1o5_x11_GLenum name );
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
    typedef void (* opengl1o5_x11_functions_glClearColor )( opengl1o5_x11_GLfloat r, opengl1o5_x11_GLfloat g, opengl1o5_x11_GLfloat b, opengl1o5_x11_GLfloat a );
    typedef void (* opengl1o5_x11_functions_glClearDepth )( opengl1o5_x11_GLdouble depth );
    typedef void (* opengl1o5_x11_functions_glClear )( opengl1o5_x11_GLbitfield mask );
    typedef void (* opengl1o5_x11_functions_glViewport )( opengl1o5_x11_GLint x, opengl1o5_x11_GLint y, opengl1o5_x11_GLsizei w, opengl1o5_x11_GLsizei h );
    typedef void (* opengl1o5_x11_functions_glDisable )( opengl1o5_x11_GLenum cap );
    typedef void (* opengl1o5_x11_functions_glEnable )( opengl1o5_x11_GLenum cap );
    typedef void (* opengl1o5_x11_functions_glPushMatrix )( void );
    typedef void (* opengl1o5_x11_functions_glPopMatrix )( void );
    typedef void (* opengl1o5_x11_functions_glMultMatrixf )( const opengl1o5_x11_GLfloat *fv );
    typedef void (* opengl1o5_x11_functions_glColor4f )( opengl1o5_x11_GLfloat r, opengl1o5_x11_GLfloat g, opengl1o5_x11_GLfloat b, opengl1o5_x11_GLfloat a );
    typedef void (* opengl1o5_x11_functions_glLightfv )( opengl1o5_x11_GLenum light, opengl1o5_x11_GLenum pname, const opengl1o5_x11_GLfloat *params );
    typedef void (* opengl1o5_x11_functions_glDepthFunc )( opengl1o5_x11_GLenum func );
    typedef void (* opengl1o5_x11_functions_glBlendFunc )( opengl1o5_x11_GLenum sfactor, opengl1o5_x11_GLenum dfactor );
    typedef void (* opengl1o5_x11_functions_glGenBuffers )( opengl1o5_x11_GLsizei n, opengl1o5_x11_GLuint *buffers );
    typedef void (* opengl1o5_x11_functions_glBindBuffer )( opengl1o5_x11_GLenum target, opengl1o5_x11_GLuint buffer );
    typedef void (* opengl1o5_x11_functions_glBufferData )( opengl1o5_x11_GLenum target, opengl1o5_x11_GLsizei size, const opengl1o5_x11_GLvoid *data, opengl1o5_x11_GLenum usage );
    typedef void (* opengl1o5_x11_functions_glDeleteBuffers )( opengl1o5_x11_GLsizei n, opengl1o5_x11_GLuint *buffers );
    typedef void (* opengl1o5_x11_functions_glFlush )( void );
    typedef opengl1o5_x11_GLenum (* opengl1o5_x11_functions_glGetError )( void );
    typedef void (* opengl1o5_x11_functions_glDeleteLists )( opengl1o5_x11_GLuint list, opengl1o5_x11_GLsizei range );
    typedef opengl1o5_x11_GLuint (* opengl1o5_x11_functions_glGenLists )( opengl1o5_x11_GLsizei range );
    typedef void (* opengl1o5_x11_functions_glNewList )( opengl1o5_x11_GLuint list, opengl1o5_x11_GLenum mode );
    typedef void (* opengl1o5_x11_functions_glEndList )( void );
    typedef void (* opengl1o5_x11_functions_glCallList )( opengl1o5_x11_GLuint list );

    struct opengl1o5_x11_functions
    {
        struct
        {
            unsigned int major, minor;
        } version;

        opengl1o5_x11_functions_glXGetProcAddress glXGetProcAddress;
        opengl1o5_x11_functions_glXSwapBuffers glXSwapBuffers;
        opengl1o5_x11_functions_glXMakeCurrent glXMakeCurrent;
        opengl1o5_x11_functions_glXCreateContext glXCreateContext;
        opengl1o5_x11_functions_glXDestroyContext glXDestroyContext;

        opengl1o5_x11_functions_glGetString glGetString;
        opengl1o5_x11_functions_glGetError glGetError;
        opengl1o5_x11_functions_glFlush glFlush;
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
        opengl1o5_x11_functions_glClearColor glClearColor;
        opengl1o5_x11_functions_glClearDepth glClearDepth;
        opengl1o5_x11_functions_glClear glClear;
        opengl1o5_x11_functions_glViewport glViewport;
        opengl1o5_x11_functions_glDisable glDisable;
        opengl1o5_x11_functions_glEnable glEnable;
        opengl1o5_x11_functions_glPushMatrix glPushMatrix;
        opengl1o5_x11_functions_glPopMatrix glPopMatrix;
        opengl1o5_x11_functions_glMultMatrixf glMultMatrixf;
        opengl1o5_x11_functions_glColor4f glColor4f;
        opengl1o5_x11_functions_glLightfv glLightfv;
        opengl1o5_x11_functions_glDepthFunc glDepthFunc;
        opengl1o5_x11_functions_glBlendFunc glBlendFunc;
        opengl1o5_x11_functions_glGenBuffers glGenBuffers;
        opengl1o5_x11_functions_glBindBuffer glBindBuffer;
        opengl1o5_x11_functions_glBufferData glBufferData;
        opengl1o5_x11_functions_glDeleteBuffers glDeleteBuffers;
        bool bUseVB;
        opengl1o5_x11_functions_glDeleteLists glDeleteLists;
        opengl1o5_x11_functions_glGenLists glGenLists;
        opengl1o5_x11_functions_glNewList glNewList;
        opengl1o5_x11_functions_glEndList glEndList;
        opengl1o5_x11_functions_glCallList glCallList;
    };

    class opengl1o5_x11 : public render_api
    {

    private:

        std::string sver, sext;
        x11_window *w;
        x11_window_Display *dpy;
        int screen;
        x11_window_Window win;
        opengl1o5_x11_GLXContext ctx, shared_ctx[ opengl1o5_x11_max_contexts ];
        int ctx_used;
        bool bIsOpen, bUseDl;
        void *dlGL;

        opengl1o5_x11_functions gl;

        //return next fee context
        opengl1o5_x11_GLXContext getNextCtx( void );
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
