
#include "opengl1o5_x11.h"
#include "opengl1o5_x11_context.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include <string.h>

namespace dragonpoop
{

    //ctor
    opengl1o5_x11::opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm, bool bUseDl ) : render_api( new x11_window( w, h, ctitle ), mm )
    {
        Window winDummy;
        unsigned int borderDummy, i;

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
        this->ctx = glXCreateContext( this->dpy, this->w->getVi(), 0, GL_TRUE );
        if( !this->ctx )
            return;
        for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
            this->shared_ctx[ i ] = glXCreateContext( this->dpy, this->w->getVi(), this->ctx, GL_TRUE );

        //male context active
        glXMakeCurrent( this->dpy, this->win, this->ctx );

        //get window position
        XGetGeometry( this->dpy, this->win, &winDummy, &this->x, &this->y, &this->width, &this->height, &borderDummy, &this->depth );

        glDisable(GL_CULL_FACE);

        glShadeModel( GL_SMOOTH );
        glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
        glClearDepth( 1.0f );

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
        glXMakeCurrent( this->dpy, this->win, this->ctx );
        this->deleteContexts();

        if( this->dpy && this->ctx )
        {
            glXMakeCurrent( this->dpy, None, 0 );
            for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
                glXDestroyContext( this->dpy, this->shared_ctx[ i ] );
            glXDestroyContext( this->dpy, this->ctx );
        }
        o.unlock();

        if( this->dlGL )
            dlclose( this->dlGL );
    }

    //load functions
    bool opengl1o5_x11::loadFunctions( void )
    {
        bool r;

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


        return r;
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
    GLXContext opengl1o5_x11::getNextCtx( void )
    {
        GLXContext r;

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
        GLXContext c;

        c = this->getNextCtx();
        if( !c )
            return 0;

        return new opengl1o5_x11_context( al, mm, c, this->win, this->dpy, this->bUseDl, &this->gl );
    }

};
