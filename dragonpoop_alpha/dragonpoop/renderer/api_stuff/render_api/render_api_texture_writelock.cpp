
#include "render_api_texture_writelock.h"
#include "render_api_texture.h"

namespace dragonpoop
{
    
    //ctor
    render_api_texture_writelock::render_api_texture_writelock( render_api_texture *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_texture_writelock::~render_api_texture_writelock( void )
    {
        
    }
    
};
