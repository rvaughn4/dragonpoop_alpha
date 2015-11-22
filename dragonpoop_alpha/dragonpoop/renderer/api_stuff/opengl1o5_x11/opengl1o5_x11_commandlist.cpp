
#include "opengl1o5_x11_commandlist.h"
#include "../render_api/render_api_context_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_shader_readlock.h"
#include "../render_api/render_api_shader_ref.h"

namespace dragonpoop
{

    //ctor
    opengl1o5_x11_commandlist::opengl1o5_x11_commandlist( dpmutex_master *mm, opengl1o5_x11_functions *gl ) : render_api_commandlist( mm )
    {
        this->dlist = 0;
        this->gl = gl;
    }

    //dtor
    opengl1o5_x11_commandlist::~opengl1o5_x11_commandlist( void )
    {
        if( this->dlist )
            this->gl->glDeleteLists( this->dlist, 1 );
    }

    //called at begin of compile
    bool opengl1o5_x11_commandlist::beginCompile( render_api_context_writelock *ctx )
    {
        this->dlist = this->gl->glGenLists( 1 );
        if( !this->dlist )
            return 0;
        this->gl->glNewList( this->dlist, GL_COMPILE );
        return 1;
    }

    //called at end of compile
    void opengl1o5_x11_commandlist::endCompile( render_api_context_writelock *ctx )
    {
        this->gl->glEndList();
        this->gl->glFlush();
    }

    //called during compile for each draw call
    bool opengl1o5_x11_commandlist::drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha, float r, float g, float b )
    {
        shared_obj_guard o;
        render_api_shader_readlock *l;
        dpmatrix mdummy;

        l = (render_api_shader_readlock *)o.tryReadLock( sdr, 100, "opengl1o5_x11_commandlist::drawCompile" );
        if( !l )
            return 0;

        return l->render( ctx, t0, t1, ib, vb, m, alpha, &mdummy, r, g, b );
    }

    //execute command list
    bool opengl1o5_x11_commandlist::execute( render_api_context_writelock *r, dpmatrix *m_world )
    {
        this->gl->glPushMatrix();
        this->gl->glMultMatrixf( m_world->getRaw4by4() );

        this->gl->glCallList( this->dlist );

        this->gl->glPopMatrix();
        return 1;
    }

};
