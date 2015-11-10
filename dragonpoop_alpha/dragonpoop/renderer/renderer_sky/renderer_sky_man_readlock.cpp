
#include "renderer_sky_man_readlock.h"
#include "renderer_sky_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_sky_man_readlock::renderer_sky_man_readlock( renderer_sky_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_sky_man_readlock::~renderer_sky_man_readlock( void )
    {
        
    }
    
    //return core
    core *renderer_sky_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
