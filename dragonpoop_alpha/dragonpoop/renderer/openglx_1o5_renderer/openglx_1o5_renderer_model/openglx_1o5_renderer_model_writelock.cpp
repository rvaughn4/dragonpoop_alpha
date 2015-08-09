
#include "openglx_1o5_renderer_model_writelock.h"
#include "openglx_1o5_renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_writelock::openglx_1o5_renderer_model_writelock( openglx_1o5_renderer_model *t, dpmutex_writelock *l ) : renderer_model_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_model_writelock::~openglx_1o5_renderer_model_writelock( void )
    {
        
    }
    
};
