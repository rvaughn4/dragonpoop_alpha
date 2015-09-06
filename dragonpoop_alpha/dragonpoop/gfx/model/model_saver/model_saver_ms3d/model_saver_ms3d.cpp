
#include "model_saver_ms3d.h"
#include "model_saver_ms3d_state_make_animation.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d::model_saver_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname ) : model_saver( c, tp, m, fname )
    {
        this->verts = 0;
        this->tris = 0;
        this->groups = 0;
        this->materials = 0;
        this->joints = 0;
        this->frames = 0;
        this->anims = 0;
    }
    
    //dtor
    model_saver_ms3d::~model_saver_ms3d( void )
    {
    }
    
    //generate second state
    model_saver_state *model_saver_ms3d::genState( dpbuffer *b )
    {
        return new model_saver_ms3d_state_make_animation( b, this->getModel() );
    }
    
};
