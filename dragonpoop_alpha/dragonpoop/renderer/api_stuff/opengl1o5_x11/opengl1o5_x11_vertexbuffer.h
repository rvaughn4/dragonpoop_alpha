
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
        unsigned int glvb;

        opengl1o5_x11_functions *gl;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //return vb
        dpvertex_buffer *getBuffer( void );
        //return vbo
        unsigned int getVBO( void );

    public:

        //ctor
        opengl1o5_x11_vertexbuffer( render_api_context_writelock *c, dpmutex_master *mm, dpvertex_buffer *vb, bool bUseDL, opengl1o5_x11_functions *gl );
        //dtor
        virtual ~opengl1o5_x11_vertexbuffer( void );

        friend class opengl1o5_x11_vertexbuffer_readlock;
    };

};

#endif
