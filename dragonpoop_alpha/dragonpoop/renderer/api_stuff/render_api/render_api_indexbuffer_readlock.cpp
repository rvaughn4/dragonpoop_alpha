
#include "render_api_indexbuffer_readlock.h"
#include "render_api_indexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_indexbuffer_readlock::render_api_indexbuffer_readlock( render_api_indexbuffer *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_indexbuffer_readlock::~render_api_indexbuffer_readlock( void )
    {
        
    }
    
};
