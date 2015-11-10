
#include "renderer_sky_man_writelock.h"
#include "renderer_sky_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_sky_man_writelock::renderer_sky_man_writelock( renderer_sky_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_sky_man_writelock::~renderer_sky_man_writelock( void )
    {
        
    }
    
    //return core
    core *renderer_sky_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run from manager thread
    void renderer_sky_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
