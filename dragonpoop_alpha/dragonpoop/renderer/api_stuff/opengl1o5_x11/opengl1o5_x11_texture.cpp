
#include "opengl1o5_x11_texture.h"
#include "../../../gfx/dpbitmap/dpbitmap.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_context_writelock.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_texture::opengl1o5_x11_texture( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpbitmap *bm ) : render_api_texture( r, c, mm, bm )
    {
        c->makeActive();
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
    
    //make texture
    void opengl1o5_x11_texture::makeTex( dpbitmap *bm )
    {
        this->glTex = 0;
    }
    
    //delete texture
    void opengl1o5_x11_texture::deleteTex( void )
    {
        glDeleteTextures( 1, &this->glTex );
    }
    
};
