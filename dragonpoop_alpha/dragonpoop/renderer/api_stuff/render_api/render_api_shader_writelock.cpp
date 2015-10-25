
#include "render_api_shader_writelock.h"
#include "render_api_shader.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader_writelock::render_api_shader_writelock( render_api_shader *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_shader_writelock::~render_api_shader_writelock( void )
    {
        
    }
    
    //render vb
    void render_api_shader_writelock::render( render_api_context_ref *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m )
    {
        this->t->render( ctx, t0, t1, ib, vb, m );
    }
    
};
