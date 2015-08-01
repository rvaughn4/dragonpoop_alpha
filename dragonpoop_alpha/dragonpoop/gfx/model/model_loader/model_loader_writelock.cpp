
#include "model_loader_writelock.h"
#include "model_loader.h"

namespace dragonpoop
{

    //ctor
    model_loader_writelock::model_loader_writelock( model_loader *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_loader_writelock::~model_loader_writelock( void )
    {
        
    }
    
    //returns true if running
    bool model_loader_writelock::isRunning( void )
    {
        return this->t->isRunning();
    }
    
    //returns true if sucessful
    bool model_loader_writelock::wasSucessful( void )
    {
        return this->t->wasSucessful();
    }
    
    //run model loader from task
    void model_loader_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
};
