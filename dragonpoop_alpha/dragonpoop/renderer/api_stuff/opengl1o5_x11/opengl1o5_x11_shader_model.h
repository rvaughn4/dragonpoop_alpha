
#ifndef dragonpoop_opengl1o5_x11_shader_model_h
#define dragonpoop_opengl1o5_x11_shader_model_h

#include "opengl1o5_x11_shader.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_shader_model : public opengl1o5_x11_shader
    {
        
    private:
        
    protected:
        
        //render vb
        virtual void render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha );
        
    public:
        
        //ctor
        opengl1o5_x11_shader_model( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_shader_model( void );
        
    };
    
};

#endif