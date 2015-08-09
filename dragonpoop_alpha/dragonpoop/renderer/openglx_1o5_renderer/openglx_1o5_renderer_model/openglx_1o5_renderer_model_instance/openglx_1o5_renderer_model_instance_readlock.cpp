
#include "openglx_1o5_renderer_model_instance_readlock.h"
#include "openglx_1o5_renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_readlock::openglx_1o5_renderer_model_instance_readlock( openglx_1o5_renderer_model_instance *t, dpmutex_readlock *l ) : renderer_model_instance_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_readlock::~openglx_1o5_renderer_model_instance_readlock( void )
    {
        
    }
    
};
