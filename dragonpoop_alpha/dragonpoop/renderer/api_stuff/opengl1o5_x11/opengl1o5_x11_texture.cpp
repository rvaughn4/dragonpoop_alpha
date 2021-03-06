
#include "opengl1o5_x11_texture.h"
#include "../../../gfx/dpbitmap/dpbitmap.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_context_writelock.h"
#include "opengl1o5_x11_texture_readlock.h"

namespace dragonpoop
{

    //ctor
    opengl1o5_x11_texture::opengl1o5_x11_texture( render_api_context_writelock *c, dpmutex_master *mm, dpbitmap *bm, opengl1o5_x11_functions *gl ) : render_api_texture( c, mm, bm )
    {
        this->gl = gl;
        this->makeTex( bm );
    }

    //dtor
    opengl1o5_x11_texture::~opengl1o5_x11_texture( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "opengl1o5_x11_texture::~opengl1o5_x11_texture" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "opengl1o5_x11_texture::~opengl1o5_x11_texture" );
        this->deleteTex();
        o.unlock();
    }

    //generate read lock
    shared_obj_readlock *opengl1o5_x11_texture::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new opengl1o5_x11_texture_readlock( (opengl1o5_x11_texture *)p, l );
    }

    //make texture
    void opengl1o5_x11_texture::makeTex( dpbitmap *bm )
    {
        unsigned int sz, bits, w, h;
        char *buffer;

        this->glTex = 0;
        sz = bm->getSize();
        w = bm->getWidth();
        h = bm->getHeight();
        bits = bm->getBitsPerPixel();
        buffer = bm->getBuffer();

        if( !buffer || !sz || !w || !h || ( bits != 24 && bits != 32 ) )
            return;

        this->gl->glGenTextures( 1, &this->glTex );
        if( !this->glTex )
            return;

        this->gl->glBindTexture( GL_TEXTURE_2D, this->glTex );
        this->gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        this->gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        if( bits == 24 )
            this->gl->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
        if( bits == 32 )
            this->gl->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );

        if( this->gl->glGetError() != 0 )
        {
            this->gl->glDeleteTextures( 1, &this->glTex );
            this->glTex = 0;
        }
    }

    //delete texture
    void opengl1o5_x11_texture::deleteTex( void )
    {
        this->gl->glDeleteTextures( 1, &this->glTex );
    }

    //return tex
    unsigned int opengl1o5_x11_texture::getTex( void )
    {
        return this->glTex;
    }

    //returns true if works
    bool opengl1o5_x11_texture::works( void )
    {
        return this->glTex != 0;
    }

};
