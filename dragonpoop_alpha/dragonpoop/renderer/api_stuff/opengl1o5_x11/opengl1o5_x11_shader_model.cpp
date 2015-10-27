
#include "opengl1o5_x11_shader_model.h"
#include "../render_api/render_api_writelock.h"
#include "../render_api/render_api_ref.h"
#include "../render_api/render_api_context_readlock.h"
#include "opengl1o5_x11_texture_readlock.h"
#include "opengl1o5_x11_indexbuffer_readlock.h"
#include "opengl1o5_x11_vertexbuffer_readlock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_vertexbuffer_ref.h"
#include "../render_api/render_api_indexbuffer_ref.h"
#include "../render_api/render_api_texture_ref.h"
#include "../../../gfx/dpvertex/dpindex_buffer.h"
#include "../../../gfx/dpvertex/dpvertex_buffer.h"
#include "../../../gfx/dpmatrix/dpmatrix.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_shader_model::opengl1o5_x11_shader_model( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm ) : opengl1o5_x11_shader( r, c, mm )
    {
        
    }
    
    //dtor
    opengl1o5_x11_shader_model::~opengl1o5_x11_shader_model( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_shader_model::~opengl1o5_x11_shader_model" );
        o.unlock();
        this->unlink();
    }
    
    //render vb
    void opengl1o5_x11_shader_model::render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha )
    {
        opengl1o5_x11_texture_readlock *t0l, *t1l;
        opengl1o5_x11_indexbuffer_readlock *ibl;
        opengl1o5_x11_vertexbuffer_readlock *vbl;
        shared_obj_guard ot0, ot1, oib, ovb;
        unsigned int gltex0, gltex1;
        dpvertex_buffer *bvb;
        dpindex_buffer *bib;
        
        vbl = (opengl1o5_x11_vertexbuffer_readlock *)ovb.tryReadLock( vb, 100, "opengl1o5_x11_shader::render" );
        ibl = (opengl1o5_x11_indexbuffer_readlock *)oib.tryReadLock( ib, 100, "opengl1o5_x11_shader::render" );
        t0l = t1l = 0;
        if( t0 )
            t0l = (opengl1o5_x11_texture_readlock *)ot0.tryReadLock( t0, 100, "opengl1o5_x11_shader::render" );
        if( t1 )
            t1l = (opengl1o5_x11_texture_readlock *)ot1.tryReadLock( t1, 100, "opengl1o5_x11_shader::render" );
        
        if( !vbl || !ibl )
            return;
        gltex0 = gltex1 = 0;
        if( t0l )
            gltex0 = t0l->getTex();
        if( t1l )
            gltex1 = t1l->getTex();
        
        bib = ibl->getBuffer();
        bvb = vbl->getBuffer();
        
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glLoadMatrixf( m->getRaw4by4() );
        glColor4f( 1.0f, 1.0f, 1.0f, alpha );
        
        glBindTexture( GL_TEXTURE_2D, gltex0 );
        this->renderVB( bvb, bib );
    }
    
};
