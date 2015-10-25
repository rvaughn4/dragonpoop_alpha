
#include "render_api_shader_readlock.h"
#include "render_api_shader.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader_readlock::render_api_shader_readlock( render_api_shader *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_shader_readlock::~render_api_shader_readlock( void )
    {
        
    }
    
    //render vb
    void render_api_shader_readlock::render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha )
    {
        this->t->render( ctx, t0, t1, ib, vb, m, alpha );
    }
    
};
