
#include "render_api_commandlist_readlock.h"
#include "render_api_commandlist.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_readlock::render_api_commandlist_readlock( render_api_commandlist *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_commandlist_readlock::~render_api_commandlist_readlock( void )
    {
        
    }
    
};
