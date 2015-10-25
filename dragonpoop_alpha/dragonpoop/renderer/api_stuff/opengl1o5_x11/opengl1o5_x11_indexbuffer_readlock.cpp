
#include "opengl1o5_x11_indexbuffer_readlock.h"
#include "opengl1o5_x11_indexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_indexbuffer_readlock::opengl1o5_x11_indexbuffer_readlock( opengl1o5_x11_indexbuffer *t, dpmutex_readlock *l ) : render_api_indexbuffer_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    opengl1o5_x11_indexbuffer_readlock::~opengl1o5_x11_indexbuffer_readlock( void )
    {
        
    }
    
    //return vb
    dpindex_buffer *opengl1o5_x11_indexbuffer_readlock::getBuffer( void )
    {
        return this->t->getBuffer();
    }
    
};
