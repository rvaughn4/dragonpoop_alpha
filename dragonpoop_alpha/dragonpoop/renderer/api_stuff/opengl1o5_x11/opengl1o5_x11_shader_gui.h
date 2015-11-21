
#ifndef dragonpoop_opengl1o5_x11_shader_gui_h
#define dragonpoop_opengl1o5_x11_shader_gui_h

#include "opengl1o5_x11_shader.h"

namespace dragonpoop
{

    class opengl1o5_x11_shader_gui : public opengl1o5_x11_shader
    {

    private:

        opengl1o5_x11_functions *gl;

    protected:

        //render gl
        virtual bool _render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b, unsigned int vbo );

    public:

        //ctor
        opengl1o5_x11_shader_gui( render_api_context_writelock *c, dpmutex_master *mm, opengl1o5_x11_functions *gl );
        //dtor
        virtual ~opengl1o5_x11_shader_gui( void );

    };

};

#endif
