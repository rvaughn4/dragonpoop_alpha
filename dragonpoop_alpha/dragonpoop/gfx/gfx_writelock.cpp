
#include "gfx_writelock.h"
#include "gfx.h"

namespace dragonpoop
{

    //ctor
    gfx_writelock::gfx_writelock( gfx *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    gfx_writelock::~gfx_writelock( void )
    {

    }

    //returns true if running
    bool gfx_writelock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *gfx_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //stop gfx task
    void gfx_writelock::kill( void )
    {
        this->t->kill();
    }

    //run gfx from task
    void gfx_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
