
#include "dpactor_writelock.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_writelock::dpactor_writelock( dpactor *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    dpactor_writelock::~dpactor_writelock( void )
    {
        
    }
    
    //run actor
    void dpactor_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //return core
    core *dpactor_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get position
    void dpactor_writelock::getPosition( dpposition *p )
    {
        this->t->getPosition( p );
    }
    
    //set position
    void dpactor_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }
    
};
