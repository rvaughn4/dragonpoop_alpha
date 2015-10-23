
#include "render_api_commandlist_writelock.h"
#include "render_api_commandlist.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_writelock::render_api_commandlist_writelock( render_api_commandlist *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_commandlist_writelock::~render_api_commandlist_writelock( void )
    {
        
    }
    
};
