
#include "opengl1o5_x11_commandlist_generic.h"
#include "opengl1o5_x11_commandlist_generic_batch.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_commandlist_generic::opengl1o5_x11_commandlist_generic( dpmutex_master *mm ) : render_api_commandlist_generic( mm )
    {

    }
    
    //dtor
    opengl1o5_x11_commandlist_generic::~opengl1o5_x11_commandlist_generic( void )
    {

    }

    //generate batch
    render_api_commandlist_generic_batch *opengl1o5_x11_commandlist_generic::genBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha, float r, float g, float b )
    {
        return new opengl1o5_x11_commandlist_generic_batch( this, sdr, t0, t1, vb, ib, m, alpha, r, g, b );
    }
    
};
