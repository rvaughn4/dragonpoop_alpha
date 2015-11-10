
#include "dpsky_man_readlock.h"
#include "dpsky_man.h"

namespace dragonpoop
{
    
    //ctor
    dpsky_man_readlock::dpsky_man_readlock( dpsky_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpsky_man_readlock::~dpsky_man_readlock( void )
    {
        
    }
    
    //return core
    core *dpsky_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //return sky stuff
    dpsky_stuff *dpsky_man_readlock::getSky( void )
    {
        return this->t->getSky();
    }
    
};
