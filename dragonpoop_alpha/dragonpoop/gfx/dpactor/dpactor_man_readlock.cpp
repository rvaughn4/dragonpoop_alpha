
#include "dpactor_man_readlock.h"
#include "dpactor_man.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_man_readlock::dpactor_man_readlock( dpactor_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpactor_man_readlock::~dpactor_man_readlock( void )
    {
        
    }
    
    //return core
    core *dpactor_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
