
#include "opengl1o5_x11_context.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_context::opengl1o5_x11_context( render_api_writelock *r, dpmutex_master *mm, GLXContext ctx, Window win, Display *dpy ) : render_api_context( r, mm )
    {
        this->ctx = ctx;
        this->win = win;
        this->dpy = dpy;
    }
    
    //dtor
    opengl1o5_x11_context::~opengl1o5_x11_context( void )
    {
        
    }
    
    //clear screen
    void opengl1o5_x11_context::clearColor( float r, float g, float b )
    {
        glClearColor( r, g, b, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
    }
    
    //clear depth
    void opengl1o5_x11_context::clearDepth( float f )
    {
        glClearDepth( f );
        glClear( GL_DEPTH_BUFFER_BIT );
    }
    
    //present backbuffer
    void opengl1o5_x11_context::flipBackBuffer( void )
    {
        glXSwapBuffers( this->dpy, this->win );
    }
    
    //make context active in thread
    void opengl1o5_x11_context::makeActive( void )
    {
        glXMakeCurrent( this->dpy, this->win, this->ctx );
    }
    
    //set viewport
    void opengl1o5_x11_context::setViewport( float w, float h )
    {
        glViewport( 0, 0, w, h );
    }

};
