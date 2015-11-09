
#include "opengl1o5_x11_shader.h"
#include "opengl1o5_x11_shader_readlock.h"
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
    opengl1o5_x11_shader::opengl1o5_x11_shader( render_api_context_writelock *c, dpmutex_master *mm ) : render_api_shader( c, mm )
    {

    }
    
    //dtor
    opengl1o5_x11_shader::~opengl1o5_x11_shader( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_shader::~opengl1o5_x11_shader" );
        o.unlock();
        this->unlink();
    }
    
    //generate read lock
    shared_obj_readlock *opengl1o5_x11_shader::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new opengl1o5_x11_shader_readlock( (opengl1o5_x11_shader *)p, l );
    }
    
    //render vb
    bool opengl1o5_x11_shader::render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b )
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
            return 0;
        if( t0 && !t0l )
            return 0;
        if( t1 && !t1l )
            return 0;
        gltex0 = gltex1 = 0;
        if( t0l )
            gltex0 = t0l->getTex();
        if( t1l )
            gltex1 = t1l->getTex();
        
        bib = ibl->getBuffer();
        bvb = vbl->getBuffer();
        
        return this->_render( ctx, gltex0, gltex1, bib, bvb, m, alpha, m_world, r, g, b );
    }
    
    //render gl
    bool opengl1o5_x11_shader::_render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b )
    {
        return 1;
    }
    
    //render vb
    void opengl1o5_x11_shader::renderVB( dpvertex_buffer *vb, dpindex_buffer *ib )
    {
        dpvertex *pv;//, *iv;
        dpindex *pi;//, *ii;
 
        
        if( sizeof( dpindex ) == sizeof( uint16_t ) )
        {
            pv = vb->getBuffer();
            pi = ib->getBuffer();
            
            glEnableClientState( GL_NORMAL_ARRAY );
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glEnableClientState( GL_VERTEX_ARRAY );
            
            glTexCoordPointer( 2, GL_FLOAT, sizeof( dpvertex ), &pv->texcoords[ 0 ] );
            glNormalPointer( GL_FLOAT, sizeof( dpvertex ), &pv->normal );
            glVertexPointer( 3, GL_FLOAT, sizeof( dpvertex ), &pv->pos );
            glDrawElements( GL_TRIANGLES, ib->getSize(), GL_UNSIGNED_SHORT, pi );
            
            return;
        }
    }
    
};
