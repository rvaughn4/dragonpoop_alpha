
#ifndef dragonpoop_opengl1o5_x11_commandlist_generic_batch_h
#define dragonpoop_opengl1o5_x11_commandlist_generic_batch_h

#include "opengl1o5_x11_commandlist_generic.h"
#include "../render_api/render_api_commandlist_generic_batch.h"
#include "../../../gfx/dpvertex/dpvertex_buffer.h"
#include "../../../gfx/dpvertex/dpindex_buffer.h"

namespace dragonpoop
{

    class opengl1o5_x11_commandlist_generic;

    class opengl1o5_x11_commandlist_generic_batch : public render_api_commandlist_generic_batch
    {

    private:

        opengl1o5_x11_commandlist_generic *l;
        render_api_shader_ref *sdr;
        dpmatrix m;
        float alpha, r, g, b;
        unsigned int t0, t1;
        dpvertex_buffer vb;
        dpindex_buffer ib;
        unsigned int vbo;

    protected:

        //execute batch
        virtual bool execute( render_api_context_writelock *ctx, dpmatrix *m_world );

    public:

        //ctor
        opengl1o5_x11_commandlist_generic_batch( opengl1o5_x11_commandlist_generic *l, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha, float r, float g, float b );
        //dtor
        virtual ~opengl1o5_x11_commandlist_generic_batch( void );

        friend class opengl1o5_x11_commandlist_generic;

    };

};

#endif
