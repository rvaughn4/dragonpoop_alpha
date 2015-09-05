
#include "model_saver_readlock.h"
#include "model_saver.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_readlock::model_saver_readlock( model_saver *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_saver_readlock::~model_saver_readlock( void )
    {
        
    }
    
    //returns true if running
    bool model_saver_readlock::isRunning( void )
    {
        return this->t->isRunning();
    }
    
    //returns true if sucessful
    bool model_saver_readlock::wasSucessful( void )
    {
        return this->t->wasSucessful();
    }
    
    //returns model
    model_ref *model_saver_readlock::getModel( void )
    {
        return this->t->getModel();
    }
};
