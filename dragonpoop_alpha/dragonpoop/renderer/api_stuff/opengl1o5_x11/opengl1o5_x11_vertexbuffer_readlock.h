
#ifndef dragonpoop_opengl1o5_x11_vertexbuffer_readlock_h
#define dragonpoop_opengl1o5_x11_vertexbuffer_readlock_h

#include "../render_api/render_api_vertexbuffer_readlock.h"

namespace dragonpoop
{

    class opengl1o5_x11_vertexbuffer;
    class dpvertex_buffer;

    class opengl1o5_x11_vertexbuffer_readlock : public render_api_vertexbuffer_readlock
    {

    private:

        opengl1o5_x11_vertexbuffer *t;

    protected:

        //ctor
        opengl1o5_x11_vertexbuffer_readlock( opengl1o5_x11_vertexbuffer *t, dpmutex_readlock *l );
        //dtor
        virtual ~opengl1o5_x11_vertexbuffer_readlock( void );

    public:

        //return vb
        dpvertex_buffer *getBuffer( void );
         //return vbo
        unsigned int getVBO( void );

        friend class opengl1o5_x11_vertexbuffer;
    };

};

#endif
