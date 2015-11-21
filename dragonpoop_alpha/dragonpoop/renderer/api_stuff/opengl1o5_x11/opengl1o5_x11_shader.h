
#ifndef dragonpoop_opengl1o5_x11_shader_h
#define dragonpoop_opengl1o5_x11_shader_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_shader.h"

namespace dragonpoop
{

    class dpvertex_buffer;
    class dpindex_buffer;

    class opengl1o5_x11_shader : public render_api_shader
    {

    private:

        opengl1o5_x11_functions *gl;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //render
        virtual bool render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b );
        //render gl
        virtual bool _render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world, float r, float g, float b, unsigned int vbo );
        //render vb
        void renderVB( dpvertex_buffer *vb, dpindex_buffer *ib, unsigned int vbo );

    public:

        //ctor
        opengl1o5_x11_shader( render_api_context_writelock *c, dpmutex_master *mm, opengl1o5_x11_functions *gl );
        //dtor
        virtual ~opengl1o5_x11_shader( void );

        friend class opengl1o5_x11_shader_readlock;
    };

};

#endif
