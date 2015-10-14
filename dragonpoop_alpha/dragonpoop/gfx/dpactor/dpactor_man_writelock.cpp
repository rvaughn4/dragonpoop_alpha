
#include "dpactor_man_writelock.h"
#include "dpactor_man.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_man_writelock::dpactor_man_writelock( dpactor_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpactor_man_writelock::~dpactor_man_writelock( void )
    {
        
    }
    
    //return core
    core *dpactor_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void dpactor_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
