
#include "dpactor_readlock.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_readlock::dpactor_readlock( dpactor *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpactor_readlock::~dpactor_readlock( void )
    {
        
    }
    
    //return core
    core *dpactor_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get position
    void dpactor_readlock::getPosition( dpposition *p )
    {
        this->t->getPosition( p );
    }
    
};
