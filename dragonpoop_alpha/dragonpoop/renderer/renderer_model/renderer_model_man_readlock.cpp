
#include "renderer_model_man_readlock.h"
#include "renderer_model_man.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_man_readlock::renderer_model_man_readlock( renderer_model_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_man_readlock::~renderer_model_man_readlock( void )
    {
        
    }
    
    //return core
    core *renderer_model_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
};
