
#include "model_loader_state_openfile.h"
#include <fstream>

namespace dragonpoop
{
    
    //ctor
    model_loader_state_openfile::model_loader_state_openfile( void )
    {
        
    }
    
    //dtor
    model_loader_state_openfile::~model_loader_state_openfile( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state_openfile::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        return 0;
    }

};
