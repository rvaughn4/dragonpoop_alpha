
#include "render_api_vertexbuffer_readlock.h"
#include "render_api_vertexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_vertexbuffer_readlock::render_api_vertexbuffer_readlock( render_api_vertexbuffer *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_vertexbuffer_readlock::~render_api_vertexbuffer_readlock( void )
    {
        
    }
    
};
