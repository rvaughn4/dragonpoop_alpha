
#include "renderer_readlock.h"
#include "renderer.h"

namespace dragonpoop
{

    //ctor
    renderer_readlock::renderer_readlock( renderer *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    renderer_readlock::~renderer_readlock( void )
    {

    }

    //returns true if running
    bool renderer_readlock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *renderer_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
