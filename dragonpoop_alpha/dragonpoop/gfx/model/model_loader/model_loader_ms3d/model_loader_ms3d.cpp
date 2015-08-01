
#include "model_loader_ms3d.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d::model_loader_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname ) : model_loader( c, tp, m, fname )
    {
        
    }
    
    //dtor
    model_loader_ms3d::~model_loader_ms3d( void )
    {
        
    }
    
    //generate second state
    model_loader_state *model_loader_ms3d::genState( dpbuffer *b )
    {
        return 0;
    }
    
};
