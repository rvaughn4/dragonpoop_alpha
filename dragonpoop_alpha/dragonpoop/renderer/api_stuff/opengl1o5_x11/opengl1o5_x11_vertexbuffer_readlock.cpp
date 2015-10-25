
#include "opengl1o5_x11_vertexbuffer_readlock.h"
#include "opengl1o5_x11_vertexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_vertexbuffer_readlock::opengl1o5_x11_vertexbuffer_readlock( opengl1o5_x11_vertexbuffer *t, dpmutex_readlock *l ) : render_api_vertexbuffer_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    opengl1o5_x11_vertexbuffer_readlock::~opengl1o5_x11_vertexbuffer_readlock( void )
    {
        
    }
    
    //return vb
    dpvertex_buffer *opengl1o5_x11_vertexbuffer_readlock::getBuffer( void )
    {
        return this->t->getBuffer();
    }
    
};
