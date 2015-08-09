
#include "openglx_1o5_renderer_model_readlock.h"
#include "openglx_1o5_renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_readlock::openglx_1o5_renderer_model_readlock( openglx_1o5_renderer_model *t, dpmutex_readlock *l ) : renderer_model_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_model_readlock::~openglx_1o5_renderer_model_readlock( void )
    {
        
    }
    
};
