
#include "dpsky_man_writelock.h"
#include "dpsky_man.h"

namespace dragonpoop
{
    
    //ctor
    dpsky_man_writelock::dpsky_man_writelock( dpsky_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpsky_man_writelock::~dpsky_man_writelock( void )
    {
        
    }
    
    //return core
    core *dpsky_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void dpsky_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
