
#include "openglx_1o5_renderer_readlock.h"
#include "openglx_1o5_renderer.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_readlock::openglx_1o5_renderer_readlock( openglx_1o5_renderer *t, dpmutex_readlock *l ) : renderer_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    openglx_1o5_renderer_readlock::~openglx_1o5_renderer_readlock( void )
    {

    }
    
};
