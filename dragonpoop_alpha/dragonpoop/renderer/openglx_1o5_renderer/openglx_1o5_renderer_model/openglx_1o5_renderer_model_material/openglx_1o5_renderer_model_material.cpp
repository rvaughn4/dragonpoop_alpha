
#include "openglx_1o5_renderer_model_material.h"
#include "../../../../gfx/model/model_material/model_material.h"
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_material::openglx_1o5_renderer_model_material( model_writelock *ml, model_material *m ) : renderer_model_material( ml, m )
    {
        this->tex.diffuse = 0;
        this->tex.alphamask = 0;
        this->tex.bumpmap = 0;
        this->tex.specularmap = 0;
        
        this->sync( ml, m );
    }
    
    //dtor
    openglx_1o5_renderer_model_material::~openglx_1o5_renderer_model_material( void )
    {
        this->killTex( &this->tex.diffuse );
        this->killTex( &this->tex.alphamask );
        this->killTex( &this->tex.bumpmap );
        this->killTex( &this->tex.specularmap );
    }
    
    //sync with group
    void openglx_1o5_renderer_model_material::sync( model_writelock *ml, model_material *m )
    {
        this->renderer_model_material::sync( ml, m );
        
        this->makeTex( &this->tex.diffuse, m->getDiffuseTexture() );
        this->makeTex( &this->tex.alphamask, m->getAlphaMaskTexture() );
        this->makeTex( &this->tex.bumpmap, m->getBumpMapTexture() );
        this->makeTex( &this->tex.specularmap, m->getSpecularMapTexture() );
    }

    //return diffuse tex
    unsigned int openglx_1o5_renderer_model_material::getDiffuseTex( void )
    {
        return this->tex.diffuse;
    }
    
    //return alpha tex
    unsigned int openglx_1o5_renderer_model_material::getAlphaTex( void )
    {
        return this->tex.alphamask;
    }
    
    //return bumpmap tex
    unsigned int openglx_1o5_renderer_model_material::getBumpTex( void )
    {
        return this->tex.bumpmap;
    }
    
    //return specular map tex
    unsigned int openglx_1o5_renderer_model_material::getSpecTex( void )
    {
        return this->tex.specularmap;
    }
    
    //release texture
    void openglx_1o5_renderer_model_material::killTex( unsigned int *t )
    {
        if( !t )
            return;
        if( *t )
            glDeleteTextures( 1, t );
        *t = 0;
    }
    
    //create and load texture
    void openglx_1o5_renderer_model_material::makeTex( unsigned int *p_tex, dpbitmap *img )
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
    
};
