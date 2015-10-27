
#include "opengl1o5_x11_shader.h"
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
    opengl1o5_x11_shader::opengl1o5_x11_shader( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm ) : render_api_shader( r, c, mm )
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
    
    //render vb
    void opengl1o5_x11_shader::render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha )
    {
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
            
            glTexCoordPointer( 2, GL_FLOAT, sizeof( dpvertex ), &pv->texcoords[ 0 ] );
            glNormalPointer( GL_FLOAT, sizeof( dpvertex ), &pv->normal );
            glVertexPointer( 3, GL_FLOAT, sizeof( dpvertex ), &pv->pos );
            glDrawElements( GL_TRIANGLES, ib->getSize(), GL_UNSIGNED_SHORT, pi );
            
            return;
        }
    }
    
};
