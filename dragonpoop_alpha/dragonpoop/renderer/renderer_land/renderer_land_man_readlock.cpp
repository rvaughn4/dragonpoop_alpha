
#include "renderer_land_man_readlock.h"
#include "renderer_land_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_land_man_readlock::renderer_land_man_readlock( renderer_land_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_land_man_readlock::~renderer_land_man_readlock( void )
    {
        
    }
    
    //return core
    core *renderer_land_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
