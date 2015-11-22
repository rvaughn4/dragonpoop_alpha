
#include "opengl1o5_x11.h"
#include "opengl1o5_x11_context.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include <string.h>
#include <sstream>

namespace dragonpoop
{

    //ctor
    opengl1o5_x11::opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm, bool bUseDl ) : render_api( new x11_window( w, h, ctitle ), mm )
    {
        unsigned int i;
        const char *c;
        std::size_t p0, p1;
        std::string s;

        this->dlGL = 0;
        this->ctx = 0;
        for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
            this->shared_ctx[ i ] = 0;
        this->bUseDl = bUseDl;
        this->ctx_used = 0;
        this->w = (x11_window *)this->getWindow();
        this->bIsOpen = this->w->isOpen();
        if( !this->bIsOpen )
            return;

        this->dpy = this->w->getDisplay();
        this->screen = this->w->getScreen();
        this->win = this->w->getWindow();
        this->bIsOpen = this->loadFunctions();
        if( !this->bIsOpen )
            return;

        //create context
        this->ctx = this->gl.glXCreateContext( this->dpy, this->w->getVi(), 0, GL_TRUE );
        if( !this->ctx )
            return;
        for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
            this->shared_ctx[ i ] = this->gl.glXCreateContext( this->dpy, this->w->getVi(), this->ctx, GL_TRUE );

        //male context active
        this->gl.glXMakeCurrent( this->dpy, this->win, this->ctx );

        c = (const char *)this->gl.glGetString( GL_VERSION );
        if( c )
            this->sver.assign( c );
        c = (const char *)this->gl.glGetString( GL_EXTENSIONS );
        if( c )
            this->sext.assign( c );

        //extract version number
        p1 = this->sver.size();
        p0 = this->sver.find( "." );
        if( p0 + 1 < p1 )
            p1 = this->sver.find( ".", p0 + 1 );
        if( p0 < this->sver.size() )
        {
            std::stringstream ss;
            s = this->sver.substr( 0, p0 );
            ss << s;
            ss >> this->gl.version.major;
        }
        if( p1 >= this->sver.size() )
            p1 = this->sver.size();
        if( p0 + 1 < this->sver.size() && p1 > p0 )
        {
            std::stringstream ss;
            s = this->sver.substr( p0 + 1, p1 - p0 - 1 );
            ss << s;
            ss >> this->gl.version.minor;
        }

        //get vbo extension
        p0 = this->sext.find( "GL_ARB_vertex_buffer_object" );
        if( p0 < this->sext.size() )
            this->gl.bUseVB &= 1;
        else
            this->gl.bUseVB = 0;


        this->bIsOpen = 1;
    }

    //dtor
    opengl1o5_x11::~opengl1o5_x11( void )
    {
        int i;
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        this->gl.glXMakeCurrent( this->dpy, this->win, this->ctx );
        this->deleteContexts();

        if( this->dpy && this->ctx )
        {
            this->gl.glXMakeCurrent( this->dpy, None, 0 );
            for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
                this->gl.glXDestroyContext( this->dpy, this->shared_ctx[ i ] );
            this->gl.glXDestroyContext( this->dpy, this->ctx );
        }
        o.unlock();

        if( this->dlGL )
            dlclose( this->dlGL );
    }

    //load functions
    bool opengl1o5_x11::loadFunctions( void )
    {
        bool r;
        std::string sext, sver;

        memset( &this->gl, 0, sizeof( this->gl ) );
        this->dlGL = dlopen( "libGL.so", RTLD_LAZY | RTLD_LOCAL );
        if( !this->dlGL )
            return 0;

        this->gl.glXGetProcAddress = (opengl1o5_x11_functions_glXGetProcAddress)dlsym( this->dlGL, "glXGetProcAddress" );
        if( !this->gl.glXGetProcAddress )
            this->gl.glXGetProcAddress = (opengl1o5_x11_functions_glXGetProcAddress)dlsym( this->dlGL, "glXGetProcAddressARB" );
        if( !this->gl.glXGetProcAddress )
            return 0;

        r = 1;
        r &= ( this->gl.glXSwapBuffers = (opengl1o5_x11_functions_glXSwapBuffers)dlsym( this->dlGL, "glXSwapBuffers" ) ) != 0;
        r &= ( this->gl.glXMakeCurrent = (opengl1o5_x11_functions_glXMakeCurrent)dlsym( this->dlGL, "glXMakeCurrent" ) ) != 0;
        r &= ( this->gl.glXCreateContext = (opengl1o5_x11_functions_glXCreateContext)dlsym( this->dlGL, "glXCreateContext" ) ) != 0;
        r &= ( this->gl.glXDestroyContext = (opengl1o5_x11_functions_glXDestroyContext)dlsym( this->dlGL, "glXDestroyContext" ) ) != 0;

        r &= ( this->gl.glGetString = (opengl1o5_x11_functions_glGetString)this->gl.glXGetProcAddress( (const unsigned char *)"glGetString" ) ) != 0;
        r &= ( this->gl.glEnableClientState = (opengl1o5_x11_functions_glEnableClientState)this->gl.glXGetProcAddress( (const unsigned char *)"glEnableClientState" ) ) != 0;
        r &= ( this->gl.glGenTextures = (opengl1o5_x11_functions_glGenTextures)this->gl.glXGetProcAddress( (const unsigned char *)"glGenTextures" ) ) != 0;
        r &= ( this->gl.glDeleteTextures = (opengl1o5_x11_functions_glDeleteTextures)this->gl.glXGetProcAddress( (const unsigned char *)"glDeleteTextures" ) ) != 0;
        r &= ( this->gl.glBindTexture = (opengl1o5_x11_functions_glBindTexture)this->gl.glXGetProcAddress( (const unsigned char *)"glBindTexture" ) ) != 0;
        r &= ( this->gl.glDrawElements = (opengl1o5_x11_functions_glDrawElements)this->gl.glXGetProcAddress( (const unsigned char *)"glDrawElements" ) ) != 0;
        r &= ( this->gl.glTexCoordPointer = (opengl1o5_x11_functions_glTexCoordPointer)this->gl.glXGetProcAddress( (const unsigned char *)"glTexCoordPointer" ) ) != 0;
        r &= ( this->gl.glNormalPointer = (opengl1o5_x11_functions_glNormalPointer)this->gl.glXGetProcAddress( (const unsigned char *)"glNormalPointer" ) ) != 0;
        r &= ( this->gl.glVertexPointer = (opengl1o5_x11_functions_glVertexPointer)this->gl.glXGetProcAddress( (const unsigned char *)"glVertexPointer" ) ) != 0;
        r &= ( this->gl.glTexImage2D = (opengl1o5_x11_functions_glTexImage2D)this->gl.glXGetProcAddress( (const unsigned char *)"glTexImage2D" ) ) != 0;
        r &= ( this->gl.glTexParameteri = (opengl1o5_x11_functions_glTexParameteri)this->gl.glXGetProcAddress( (const unsigned char *)"glTexParameteri" ) ) != 0;
        r &= ( this->gl.glClearColor = (opengl1o5_x11_functions_glClearColor)this->gl.glXGetProcAddress( (const unsigned char *)"glClearColor" ) ) != 0;
        r &= ( this->gl.glClearDepth = (opengl1o5_x11_functions_glClearDepth)this->gl.glXGetProcAddress( (const unsigned char *)"glClearDepth" ) ) != 0;
        r &= ( this->gl.glClear = (opengl1o5_x11_functions_glClear)this->gl.glXGetProcAddress( (const unsigned char *)"glClear" ) ) != 0;
        r &= ( this->gl.glViewport = (opengl1o5_x11_functions_glViewport)this->gl.glXGetProcAddress( (const unsigned char *)"glViewport" ) ) != 0;
        r &= ( this->gl.glDisable = (opengl1o5_x11_functions_glDisable)this->gl.glXGetProcAddress( (const unsigned char *)"glDisable" ) ) != 0;
        r &= ( this->gl.glEnable = (opengl1o5_x11_functions_glEnable)this->gl.glXGetProcAddress( (const unsigned char *)"glEnable" ) ) != 0;
        r &= ( this->gl.glPushMatrix = (opengl1o5_x11_functions_glPushMatrix)this->gl.glXGetProcAddress( (const unsigned char *)"glPushMatrix" ) ) != 0;
        r &= ( this->gl.glPopMatrix = (opengl1o5_x11_functions_glPopMatrix)this->gl.glXGetProcAddress( (const unsigned char *)"glPopMatrix" ) ) != 0;
        r &= ( this->gl.glMultMatrixf = (opengl1o5_x11_functions_glMultMatrixf)this->gl.glXGetProcAddress( (const unsigned char *)"glMultMatrixf" ) ) != 0;
        r &= ( this->gl.glColor4f = (opengl1o5_x11_functions_glColor4f)this->gl.glXGetProcAddress( (const unsigned char *)"glColor4f" ) ) != 0;
        r &= ( this->gl.glLightfv = (opengl1o5_x11_functions_glLightfv)this->gl.glXGetProcAddress( (const unsigned char *)"glLightfv" ) ) != 0;
        r &= ( this->gl.glDepthFunc = (opengl1o5_x11_functions_glDepthFunc)this->gl.glXGetProcAddress( (const unsigned char *)"glDepthFunc" ) ) != 0;
        r &= ( this->gl.glBlendFunc = (opengl1o5_x11_functions_glBlendFunc)this->gl.glXGetProcAddress( (const unsigned char *)"glBlendFunc" ) ) != 0;
        r &= ( this->gl.glFlush = (opengl1o5_x11_functions_glFlush)this->gl.glXGetProcAddress( (const unsigned char *)"glFlush" ) ) != 0;
        r &= ( this->gl.glGetError = (opengl1o5_x11_functions_glGetError)this->gl.glXGetProcAddress( (const unsigned char *)"glGetError" ) ) != 0;
        r &= ( this->gl.glDeleteLists = (opengl1o5_x11_functions_glDeleteLists)this->gl.glXGetProcAddress( (const unsigned char *)"glDeleteLists" ) ) != 0;
        r &= ( this->gl.glGenLists = (opengl1o5_x11_functions_glGenLists)this->gl.glXGetProcAddress( (const unsigned char *)"glGenLists" ) ) != 0;
        r &= ( this->gl.glNewList = (opengl1o5_x11_functions_glNewList)this->gl.glXGetProcAddress( (const unsigned char *)"glNewList" ) ) != 0;
        r &= ( this->gl.glEndList = (opengl1o5_x11_functions_glEndList)this->gl.glXGetProcAddress( (const unsigned char *)"glEndList" ) ) != 0;
        r &= ( this->gl.glCallList = (opengl1o5_x11_functions_glCallList)this->gl.glXGetProcAddress( (const unsigned char *)"glCallList" ) ) != 0;

        if( !r )
            return 0;

        this->gl.glGenBuffers = (opengl1o5_x11_functions_glGenBuffers)this->gl.glXGetProcAddress( (const unsigned char *)"glGenBuffers" );
        if( !this->gl.glGenBuffers )
            this->gl.glGenBuffers = (opengl1o5_x11_functions_glGenBuffers)this->gl.glXGetProcAddress( (const unsigned char *)"glGenBuffersARB" );
        this->gl.glBindBuffer = (opengl1o5_x11_functions_glBindBuffer)this->gl.glXGetProcAddress( (const unsigned char *)"glBindBuffer" );
        if( !this->gl.glBindBuffer )
            this->gl.glBindBuffer = (opengl1o5_x11_functions_glBindBuffer)this->gl.glXGetProcAddress( (const unsigned char *)"glBindBufferARB" );
        this->gl.glBufferData = (opengl1o5_x11_functions_glBufferData)this->gl.glXGetProcAddress( (const unsigned char *)"glBufferData" );
        if( !this->gl.glBufferData )
            this->gl.glBufferData = (opengl1o5_x11_functions_glBufferData)this->gl.glXGetProcAddress( (const unsigned char *)"glBufferDataARB" );
        this->gl.glDeleteBuffers = (opengl1o5_x11_functions_glDeleteBuffers)this->gl.glXGetProcAddress( (const unsigned char *)"glDeleteBuffers" );
        if( !this->gl.glDeleteBuffers )
            this->gl.glDeleteBuffers = (opengl1o5_x11_functions_glDeleteBuffers)this->gl.glXGetProcAddress( (const unsigned char *)"glDeleteBuffersARB" );
        this->gl.bUseVB = this->gl.glGenBuffers != 0 && this->gl.glDeleteBuffers != 0 && this->gl.glBindBuffer != 0 && this->gl.glBufferData != 0;

        return 1;
    }

    //run api
    void opengl1o5_x11::run( void )
    {
        this->render_api::run();
    }

    //returns true if window is open
    bool opengl1o5_x11::isOpen( void )
    {
        if( !this->render_api::isOpen() )
            return 0;
        return this->bIsOpen;
    }

    //return next fee context
    opengl1o5_x11_GLXContext opengl1o5_x11::getNextCtx( void )
    {
        opengl1o5_x11_GLXContext r;

        while( this->ctx_used < opengl1o5_x11_max_contexts )
        {
            r = this->shared_ctx[ this->ctx_used ];
            this->ctx_used++;
            if( r )
                return r;
        }

        return 0;
    }

    //generate context
    render_api_context *opengl1o5_x11::genContext( render_api_writelock *al, dpmutex_master *mm )
    {
        opengl1o5_x11_GLXContext c;

        c = this->getNextCtx();
        if( !c )
            return 0;

        return new opengl1o5_x11_context( al, mm, c, this->win, this->dpy, this->bUseDl, &this->gl );
    }

};
