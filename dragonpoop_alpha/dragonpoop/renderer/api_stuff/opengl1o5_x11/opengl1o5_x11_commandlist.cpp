
#include "opengl1o5_x11_commandlist.h"
#include "../render_api/render_api_context_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../render_api/render_api_shader_readlock.h"
#include "../render_api/render_api_shader_ref.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_commandlist::opengl1o5_x11_commandlist( dpmutex_master *mm ) : render_api_commandlist( mm )
    {
        this->dlist = 0;
    }
    
    //dtor
    opengl1o5_x11_commandlist::~opengl1o5_x11_commandlist( void )
    {
        if( this->dlist )
            glDeleteLists( this->dlist, 1 );
    }
    
    //called at begin of compile
    bool opengl1o5_x11_commandlist::beginCompile( render_api_context_writelock *ctx )
    {
        ctx->makeActive();
        this->dlist = glGenLists( 1 );
        if( !this->dlist )
            return 0;
        glNewList( this->dlist, GL_COMPILE );
        return 1;
    }
    
    //called at end of compile
    void opengl1o5_x11_commandlist::endCompile( render_api_context_writelock *ctx )
    {
        glEndList();
    }
    
    //called during compile for each draw call
    bool opengl1o5_x11_commandlist::drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha )
    {
        shared_obj_guard o;
        render_api_shader_readlock *l;
        
        l = (render_api_shader_readlock *)o.tryReadLock( sdr, 100, "opengl1o5_x11_commandlist::drawCompile" );
        if( !l )
            return 0;
        
        return l->render( ctx, t0, t1, ib, vb, m, alpha );
    }
    
    //execute command list
    bool opengl1o5_x11_commandlist::execute( render_api_context_writelock *r )
    {
        glCallList( this->dlist );
        return 1;
    }
    
};
