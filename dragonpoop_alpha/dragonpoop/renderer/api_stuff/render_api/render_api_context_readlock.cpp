
#include "render_api_context_readlock.h"
#include "render_api_context.h"

namespace dragonpoop
{
    
    //ctor
    render_api_context_readlock::render_api_context_readlock( render_api_context *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_context_readlock::~render_api_context_readlock( void )
    {
        
    }
    
    //return width and height
    void render_api_context_readlock::getDimensions( float *w, float *h )
    {
        this->t->getDimensions( w, h );
    }
    
};
