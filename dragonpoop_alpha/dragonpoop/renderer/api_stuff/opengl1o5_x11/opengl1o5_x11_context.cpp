
#include "opengl1o5_x11_context.h"
#include "../render_api/render_api_context.h"
#include "opengl1o5_x11_commandlist.h"
#include "opengl1o5_x11_commandlist_generic.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "opengl1o5_x11_texture.h"
#include "opengl1o5_x11_vertexbuffer.h"
#include "opengl1o5_x11_indexbuffer.h"
#include "opengl1o5_x11_shader_gui.h"
#include "opengl1o5_x11_shader_model.h"
#include "opengl1o5_x11_shader_land.h"
#include "opengl1o5_x11_shader_sky.h"
#include "../../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{

    //ctor
    opengl1o5_x11_context::opengl1o5_x11_context( render_api_writelock *r, dpmutex_master *mm, GLXContext ctx, Window win, Display *dpy, bool bUseDl, opengl1o5_x11_functions *gl ) : render_api_context( r, mm )
    {
        this->bUseDl = bUseDl;
        this->ctx = ctx;
        this->win = win;
        this->dpy = dpy;
        this->tid = 0;
        this->gl = gl;
    }

    //dtor
    opengl1o5_x11_context::~opengl1o5_x11_context( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        this->deleteIndexBuffers();
        this->deleteShaders();
        this->deleteTextures();
        this->deleteVertexBuffers();
        o.unlock();
    }

    //generate commandlist
    render_api_commandlist *opengl1o5_x11_context::genCmdList( render_api_context_writelock *cl, dpmutex_master *mm )
    {
        if( this->bUseDl )
            return new opengl1o5_x11_commandlist( mm );
        return new opengl1o5_x11_commandlist_generic( mm );
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
    void opengl1o5_x11_context::makeActive( dpthread_lock *thd )
    {
        if( thd->getId() == this->tid )
            return;
            this->tid = thd->getId();
        glXMakeCurrent( this->dpy, this->win, this->ctx );
    }

    //set viewport
    void opengl1o5_x11_context::setViewport( float w, float h )
    {
        glViewport( 0, 0, w, h );
    }

    //gen shader
    render_api_shader *opengl1o5_x11_context::genShader( dpmutex_master *mm, render_api_context_writelock *cl, const char *cname )
    {
        std::string s;

        s.assign( cname );

        if( s.compare( "gui" ) == 0 )
            return new opengl1o5_x11_shader_gui( cl, mm );
        if( s.compare( "model" ) == 0 )
            return new opengl1o5_x11_shader_model( cl, mm );
        if( s.compare( "land" ) == 0 )
            return new opengl1o5_x11_shader_land( cl, mm );
        if( s.compare( "sky" ) == 0 )
            return new opengl1o5_x11_shader_sky( cl, mm );

        return 0;
    }

    //gen texture
    render_api_texture *opengl1o5_x11_context::genTexture( dpmutex_master *mm, render_api_context_writelock *cl, dpbitmap *bm )
    {
        return new opengl1o5_x11_texture( cl, mm, bm, this->gl );
    }

    //gen vertex buffer
    render_api_vertexbuffer *opengl1o5_x11_context::genVertexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        return new opengl1o5_x11_vertexbuffer( cl, mm, vb );
    }

    //gen index buffer
    render_api_indexbuffer *opengl1o5_x11_context::genIndexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpindex_buffer *ib )
    {
        return new opengl1o5_x11_indexbuffer( cl, mm, ib );
    }

};
