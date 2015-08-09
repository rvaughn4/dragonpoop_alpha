
#include "openglx_1o5_renderer_model_instance_writelock.h"
#include "openglx_1o5_renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_writelock::openglx_1o5_renderer_model_instance_writelock( openglx_1o5_renderer_model_instance *t, dpmutex_writelock *l ) : renderer_model_instance_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_writelock::~openglx_1o5_renderer_model_instance_writelock( void )
    {
        
    }
    
};
