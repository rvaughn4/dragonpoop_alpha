
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
        
    protected:
        
        //render vb
        virtual void render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha );
        //render vb
        void renderVB( dpvertex_buffer *vb, dpindex_buffer *ib );
        
    public:
        
        //ctor
        opengl1o5_x11_shader( render_api_context_writelock *c, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_shader( void );
        
    };
    
};

#endif