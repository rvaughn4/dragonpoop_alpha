
#include "render_api_context_writelock.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    //ctor
    render_api_context_writelock::render_api_context_writelock( render_api_context *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_context_writelock::~render_api_context_writelock( void )
    {
        
    }
    
};
