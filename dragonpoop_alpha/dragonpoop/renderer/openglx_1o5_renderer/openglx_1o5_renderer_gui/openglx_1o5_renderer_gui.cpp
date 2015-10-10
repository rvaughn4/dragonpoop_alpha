
#include "openglx_1o5_renderer_gui.h"
#include "openglx_1o5_renderer_gui_ref.h"
#include "openglx_1o5_renderer_gui_readlock.h"
#include "openglx_1o5_renderer_gui_writelock.h"
#include "../openglx_1o5_renderer.h"

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_gui::openglx_1o5_renderer_gui( gui_writelock *g, openglx_1o5_renderer *r ) : renderer_gui( g )
    {
        this->fg_tex = this->bg_tex = 0;
        this->r = r;
    }
    
    //dtor
    openglx_1o5_renderer_gui::~openglx_1o5_renderer_gui( void )
    {
        this->killTex( &this->fg_tex, &this->fg_tex_sz );
        this->killTex( &this->bg_tex, &this->bg_tex_sz );
    }
    
    //generate read lock
    shared_obj_readlock *openglx_1o5_renderer_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new openglx_1o5_renderer_gui_readlock( (openglx_1o5_renderer_gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *openglx_1o5_renderer_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new openglx_1o5_renderer_gui_writelock( (openglx_1o5_renderer_gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *openglx_1o5_renderer_gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new openglx_1o5_renderer_gui_ref( (openglx_1o5_renderer_gui *)p, k );
    }
    
    //override to handle bg texture update
    void openglx_1o5_renderer_gui::updateBg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        this->makeTex( &this->bg_tex, &this->bg_tex_sz, bm );
    }
    
    //override to handle fg texture update
    void openglx_1o5_renderer_gui::updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        this->makeTex( &this->fg_tex, &this->fg_tex_sz, bm );
    }
    
    //release texture
    void openglx_1o5_renderer_gui::killTex( unsigned int *t, unsigned int *psz )
    {
        if( !t )
            return;
        if( *t )
        {
            glDeleteTextures( 1, t );
            this->r->subTexMemory( *psz );
        }
        *psz = 0;
        *t = 0;
    }
    
    //create and load texture
    void openglx_1o5_renderer_gui::makeTex( unsigned int *p_tex, unsigned int *psz, dpbitmap *img )
    {
        unsigned int sz, bits, w, h;
        char *buffer;
        bool doLin;
        
        this->killTex( p_tex, psz );
        doLin = 1;
        
        sz = img->getSize();
        w = img->getWidth();
        h = img->getHeight();
        bits = img->getBitsPerPixel();
        buffer = img->getBuffer();
        *psz = 0;
        
        if( !p_tex || !buffer || !sz || !w || !h || ( bits != 24 && bits != 32 ) )
            return;
        
        glGenTextures( 1, p_tex );
        if( !*p_tex )
            return;
        
        glBindTexture( GL_TEXTURE_2D, *p_tex );
        
        if( doLin )
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        }
        else
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        }
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
            
        buffer = img->getBuffer();
        *psz += img->getSize();
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, img->getWidth(), img->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
        
        this->r->addTexMemory( *psz );
    }
    
    //return bg texture
    unsigned int openglx_1o5_renderer_gui::getBgTex( void )
    {
        return this->bg_tex;
    }
    
    //return fg texture
    unsigned int openglx_1o5_renderer_gui::getFgTex( void )
    {
        return this->fg_tex;
    }
    
};
