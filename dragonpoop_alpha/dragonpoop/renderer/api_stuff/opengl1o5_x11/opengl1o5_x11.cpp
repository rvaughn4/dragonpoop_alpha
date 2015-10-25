
#include "opengl1o5_x11.h"
#include "opengl1o5_x11_context.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "opengl1o5_x11_shader.h"
#include "opengl1o5_x11_texture.h"
#include "opengl1o5_x11_vertexbuffer.h"
#include "opengl1o5_x11_indexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11::opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm ) : render_api( new x11_window( w, h, ctitle ), mm )
    {
        Window winDummy;
        unsigned int borderDummy, i;

        this->ctx_used = 0;
        this->w = (x11_window *)this->getWindow();
        this->bIsOpen = this->w->isOpen();
        if( !this->bIsOpen )
            return;
        
        this->dpy = this->w->getDisplay();
        this->screen = this->w->getScreen();
        this->win = this->w->getWindow();
    
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
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glEnable( GL_TEXTURE_2D );
        
        glEnableClientState( GL_NORMAL_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glEnableClientState( GL_VERTEX_ARRAY );
        
        this->bIsOpen = 1;
    }
    
    //dtor
    opengl1o5_x11::~opengl1o5_x11( void )
    {
        int i;
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        this->unlink();
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11::~opengl1o5_x11" );
        glXMakeCurrent( this->dpy, this->win, this->ctx );
        this->deleteIndexBuffers();
        this->deleteShaders();
        this->deleteTextures();
        this->deleteVertexBuffers();
        this->deleteContexts();
        
        if( this->dpy && this->ctx )
        {
            glXMakeCurrent( this->dpy, None, 0 );
            for( i = 0; i < opengl1o5_x11_max_contexts; i++ )
                glXDestroyContext( this->dpy, this->shared_ctx[ i ] );
            glXDestroyContext( this->dpy, this->ctx );
        }
        o.unlock();

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
        
        return new opengl1o5_x11_context( al, mm, c, this->win, this->dpy );
    }

    //gen shader
    render_api_shader *opengl1o5_x11::genShader( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, const char *cname )
    {
        return 0;
    }
    
    //gen texture
    render_api_texture *opengl1o5_x11::genTexture( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm )
    {
        return new opengl1o5_x11_texture( al, cl, mm, bm );
    }
    
    //gen vertex buffer
    render_api_vertexbuffer *opengl1o5_x11::genVertexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        return new opengl1o5_x11_vertexbuffer( al, cl, mm, vb );
    }
    
    //gen index buffer
    render_api_indexbuffer *opengl1o5_x11::genIndexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *ib )
    {
        return new opengl1o5_x11_indexbuffer( al, cl, mm, ib );
    }
    
};
