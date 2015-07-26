
#include "openglx_1o5_renderer_writelock.h"
#include "openglx_1o5_renderer.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer_writelock::openglx_1o5_renderer_writelock( openglx_1o5_renderer *t, dpmutex_writelock *l ) : renderer_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    openglx_1o5_renderer_writelock::~openglx_1o5_renderer_writelock( void )
    {

    }
    
};
