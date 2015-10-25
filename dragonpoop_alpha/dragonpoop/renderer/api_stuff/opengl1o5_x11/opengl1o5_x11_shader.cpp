
#include "opengl1o5_x11_shader.h"
#include "../render_api/render_api_writelock.h"
#include "../render_api/render_api_ref.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_shader::opengl1o5_x11_shader( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm ) : render_api_shader( r, c, mm )
    {

    }
    
    //dtor
    opengl1o5_x11_shader::~opengl1o5_x11_shader( void )
    {

    }
    
    //render vb
    void opengl1o5_x11_shader::render( render_api_context_ref *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m )
    {
        
    }
    
};
