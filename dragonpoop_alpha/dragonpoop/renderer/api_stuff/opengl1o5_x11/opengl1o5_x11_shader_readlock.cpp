
#include "opengl1o5_x11_shader_readlock.h"
#include "opengl1o5_x11_shader.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_shader_readlock::opengl1o5_x11_shader_readlock( opengl1o5_x11_shader *t, dpmutex_readlock *l ) : render_api_shader_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    opengl1o5_x11_shader_readlock::~opengl1o5_x11_shader_readlock( void )
    {
        
    }
    
    //render gl
    bool opengl1o5_x11_shader_readlock::_render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha )
    {
        return this->t->_render( ctx, t0, t1, ib, vb, m, alpha );
    }

};
