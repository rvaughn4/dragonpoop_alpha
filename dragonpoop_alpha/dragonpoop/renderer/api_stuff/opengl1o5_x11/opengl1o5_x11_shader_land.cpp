
#include "opengl1o5_x11_shader_land.h"
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
    opengl1o5_x11_shader_land::opengl1o5_x11_shader_land( render_api_context_writelock *c, dpmutex_master *mm, opengl1o5_x11_functions *gl ) : opengl1o5_x11_shader( c, mm, gl )
    {
        this->gl = gl;
    }

    //dtor
    opengl1o5_x11_shader_land::~opengl1o5_x11_shader_land( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "opengl1o5_x11_shader_land::~opengl1o5_x11_shader_land" );
        o.unlock();
        this->unlink();
    }

    //render gl
    bool opengl1o5_x11_shader_land::_render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b, unsigned int vbo )
    {

        float LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
        float LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
        float LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };
        this->gl->glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
        this->gl->glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
        this->gl->glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
        this->gl->glEnable(GL_LIGHT1);								// Enable Light One
        this->gl->glEnable( GL_LIGHTING );

        this->gl->glEnable( GL_DEPTH_TEST );
        this->gl->glDepthFunc( GL_LEQUAL );
        this->gl->glEnable( GL_TEXTURE_2D );
        this->gl->glDisable( GL_BLEND );

        this->gl->glPushMatrix();
        this->gl->glMultMatrixf( m_world->getRaw4by4() );
        this->gl->glMultMatrixf( m->getRaw4by4() );
        this->gl->glColor4f( r, g, b, alpha );

        this->gl->glBindTexture( GL_TEXTURE_2D, t0 );
        this->renderVB( vb, ib, vbo );

        this->gl->glPopMatrix();

        return 1;
    }

};
