
#include "opengl1o5_x11_shader_gui.h"
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
    opengl1o5_x11_shader_gui::opengl1o5_x11_shader_gui( render_api_context_writelock *c, dpmutex_master *mm ) : opengl1o5_x11_shader( c, mm )
    {
        
    }
    
    //dtor
    opengl1o5_x11_shader_gui::~opengl1o5_x11_shader_gui( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_shader_gui::~opengl1o5_x11_shader_gui" );
        o.unlock();
        this->unlink();
    }
    
    //render gl
    bool opengl1o5_x11_shader_gui::_render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world )
    {

        glDisable( GL_LIGHTING );
        glDisable( GL_DEPTH_TEST );
        glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        glPushMatrix();
        glMultMatrixf( m_world->getRaw4by4() );
        glMultMatrixf( m->getRaw4by4() );
        glColor4f( 1.0f, 1.0f, 1.0f, alpha );
        
        glBindTexture( GL_TEXTURE_2D, t0 );
        this->renderVB( vb, ib );
        
        glPopMatrix();
        
        return 1;
    }
    
};
