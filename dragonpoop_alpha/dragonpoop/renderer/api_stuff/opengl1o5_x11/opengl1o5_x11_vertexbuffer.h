
#ifndef dragonpoop_opengl1o5_x11_vertexbuffer_h
#define dragonpoop_opengl1o5_x11_vertexbuffer_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_vertexbuffer.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_vertexbuffer : public render_api_vertexbuffer
    {
        
    private:
        
        dpvertex_buffer vb;
        
    protected:
        
    public:
        
        //ctor
        opengl1o5_x11_vertexbuffer( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpvertex_buffer *vb );
        //dtor
        virtual ~opengl1o5_x11_vertexbuffer( void );
        
    };
    
};

#endif