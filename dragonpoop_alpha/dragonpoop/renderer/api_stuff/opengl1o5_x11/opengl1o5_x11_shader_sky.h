
#ifndef dragonpoop_opengl1o5_x11_shader_sky_h
#define dragonpoop_opengl1o5_x11_shader_sky_h

#include "opengl1o5_x11_shader.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_shader_sky : public opengl1o5_x11_shader
    {
        
    private:
        
    protected:
        
        //render gl
        virtual bool _render( render_api_context_writelock *ctx, unsigned int t0, unsigned int t1, dpindex_buffer *ib, dpvertex_buffer *vb, dpmatrix *m, float alpha );

    public:
        
        //ctor
        opengl1o5_x11_shader_sky( render_api_context_writelock *c, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_shader_sky( void );
        
    };
    
};

#endif