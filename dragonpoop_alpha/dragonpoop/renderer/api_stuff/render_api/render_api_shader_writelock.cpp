
#include "render_api_shader_writelock.h"
#include "render_api_shader.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader_writelock::render_api_shader_writelock( render_api_shader *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_shader_writelock::~render_api_shader_writelock( void )
    {
        
    }
    
};
