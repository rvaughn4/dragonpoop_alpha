
#include "render_api_indexbuffer_writelock.h"
#include "render_api_indexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_indexbuffer_writelock::render_api_indexbuffer_writelock( render_api_indexbuffer *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_indexbuffer_writelock::~render_api_indexbuffer_writelock( void )
    {
        
    }
    
};
