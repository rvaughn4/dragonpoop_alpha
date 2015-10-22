
#include "render_api_vertexbuffer_writelock.h"
#include "render_api_vertexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_vertexbuffer_writelock::render_api_vertexbuffer_writelock( render_api_vertexbuffer *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_vertexbuffer_writelock::~render_api_vertexbuffer_writelock( void )
    {
        
    }
    
};
