
#include "model_loader_ms3d.h"
#include "model_loader_ms3d_state_parse_header.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d::model_loader_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname ) : model_loader( c, tp, m, fname )
    {
        this->verts = 0;
        this->tris = 0;
        this->groups = 0;
        this->materials = 0;
        this->joints = 0;
        this->frames = 0;
    }
    
    //dtor
    model_loader_ms3d::~model_loader_ms3d( void )
    {
    }
    
    //generate second state
    model_loader_state *model_loader_ms3d::genState( dpbuffer *b )
    {
        return new model_loader_ms3d_state_parse_header( b );
    }
    
};
