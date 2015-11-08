
#ifndef dragonpoop_opengl1o5_x11_shader_readlock_h
#define dragonpoop_opengl1o5_x11_shader_readlock_h

#include "../render_api/render_api_shader_readlock.h"

namespace dragonpoop
{
    class opengl1o5_x11_shader;
    class dpindex_buffer;
    class dpvertex_buffer;
    
    class opengl1o5_x11_shader_readlock : public render_api_shader_readlock
    {
        
    private:
        
        opengl1o5_x11_shader *t;
        
    protected:
        
        //ctor
        opengl1o5_x11_shader_readlock( opengl1o5_x11_shader *t, dpmutex_readlock *l );
        //dtor
        virtual ~opengl1o5_x11_shader_readlock( void );
        
    public:
        
        //render gl
        virtual bool _render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha, dpmatrix *m_world );

        friend class opengl1o5_x11_shader;
    };
    
};

#endif