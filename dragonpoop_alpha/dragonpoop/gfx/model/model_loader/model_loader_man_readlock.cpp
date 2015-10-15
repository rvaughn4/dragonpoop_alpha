
#include "model_loader_man_readlock.h"
#include "model_loader_man.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_man_readlock::model_loader_man_readlock( model_loader_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_loader_man_readlock::~model_loader_man_readlock( void )
    {
        
    }
    
    //return core
    core *model_loader_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
