
#include "render_api_texture_readlock.h"
#include "render_api_texture.h"

namespace dragonpoop
{

    //ctor
    render_api_texture_readlock::render_api_texture_readlock( render_api_texture *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    render_api_texture_readlock::~render_api_texture_readlock( void )
    {

    }

    //returns true if works
    bool render_api_texture_readlock::works( void )
    {
        return this->t->works();
    }

};
