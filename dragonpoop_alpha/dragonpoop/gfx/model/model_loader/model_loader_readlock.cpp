
#include "model_loader_readlock.h"
#include "model_loader.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_readlock::model_loader_readlock( model_loader *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_loader_readlock::~model_loader_readlock( void )
    {
        
    }
    
    //returns true if running
    bool model_loader_readlock::isRunning( void )
    {
        return this->t->isRunning();
    }
    
    //returns true if sucessful
    bool model_loader_readlock::wasSucessful( void )
    {
        return this->t->wasSucessful();
    }
    
    //returns model
    model_ref *model_loader_readlock::getModel( void )
    {
        return this->t->getModel();
    }
};
