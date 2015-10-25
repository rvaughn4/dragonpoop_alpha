
#ifndef dragonpoop_opengl1o5_x11_shader_h
#define dragonpoop_opengl1o5_x11_shader_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_shader.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_shader : public render_api_shader
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        opengl1o5_x11_shader( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_shader( void );
        
    };
    
};

#endif