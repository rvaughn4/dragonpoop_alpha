
#include "opengl1o5_x11_texture_readlock.h"
#include "opengl1o5_x11_texture.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_texture_readlock::opengl1o5_x11_texture_readlock( opengl1o5_x11_texture *t, dpmutex_readlock *l ) : render_api_texture_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    opengl1o5_x11_texture_readlock::~opengl1o5_x11_texture_readlock( void )
    {
        
    }
    
    //return tex
    unsigned int opengl1o5_x11_texture_readlock::getTex( void )
    {
        return this->t->getTex();
    }
    
};
