
#include "openglx_1o5_renderer_gui.h"
#include "openglx_1o5_renderer_gui_ref.h"
#include "openglx_1o5_renderer_gui_readlock.h"
#include "openglx_1o5_renderer_gui_writelock.h"
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_gui::openglx_1o5_renderer_gui( gui_writelock *g ) : renderer_gui( g )
    {
        this->fg_tex = this->bg_tex = 0;
    }
    
    //dtor
    openglx_1o5_renderer_gui::~openglx_1o5_renderer_gui( void )
    {
        this->killTex( &this->fg_tex );
        this->killTex( &this->bg_tex );
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
        this->makeTex( &this->bg_tex, bm );
    }
    
    //override to handle fg texture update
    void openglx_1o5_renderer_gui::updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        this->makeTex( &this->fg_tex, bm );
    }
    
    //release texture
    void openglx_1o5_renderer_gui::killTex( unsigned int *t )
    {
        if( !t )
            return;
        if( *t )
            glDeleteTextures( 1, t );
        *t = 0;
    }
    
    //create and load texture
    void openglx_1o5_renderer_gui::makeTex( unsigned int *p_tex, dpbitmap *img )
    {
        unsigned int sz, bits, w, h, d, l, m;
        char *buffer;
        bool doMip, doLin;
        dpbitmap ci;
        
        this->killTex( p_tex );
        
        sz = img->getSize();
        w = img->getWidth();
        h = img->getHeight();
        bits = img->getBitsPerPixel();
        buffer = img->getBuffer();
        
        if( !p_tex || !buffer || !sz || !w || !h || ( bits != 24 && bits != 32 ) )
            return;
        
        doMip = 1;
        doLin = 1;
        
        glGenTextures( 1, p_tex );
        if( !*p_tex )
            return;
        
        glBindTexture( GL_TEXTURE_2D, *p_tex );
        
        if( doLin )
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            if( doMip )
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            else
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        }
        else
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            if( doMip )
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
            else
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        }
        
        l = w;
        if( h > l )
            l = h;
        if( l > 512 )
            l = 512;
        d = 8;
        while( d < l )
            d *= 8;
        
        if( doMip )
        {
            
            m = 0;
            l = d * 2;
            do
            {
                l = l / 2;
                m++;
            }
            while( l > 1 );
            m--;
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m );
            
            d *= 2;
            l = 0;
            do
            {
                d /= 2;
                
                ci.reset();
                if( !ci.resize( d, d ) )
                    d = 0;
                else
                {
                    ci.blit( img, 0 );
                    buffer = ci.getBuffer();
                    glTexImage2D( GL_TEXTURE_2D, l, GL_RGBA8, d, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
                    l++;
                }
            }
            while( d > 1 );
        }
        else
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
            
            ci.reset();
            if( !ci.resize( d, d ) )
                d = 0;
            else
            {
                
                ci.blit( img, 0 );
                buffer = ci.getBuffer();
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, d, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
            }
        }
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
