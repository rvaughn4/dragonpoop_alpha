
#include "render_api_shader_readlock.h"
#include "render_api_shader.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader_readlock::render_api_shader_readlock( render_api_shader *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_shader_readlock::~render_api_shader_readlock( void )
    {
        
    }
    
};
