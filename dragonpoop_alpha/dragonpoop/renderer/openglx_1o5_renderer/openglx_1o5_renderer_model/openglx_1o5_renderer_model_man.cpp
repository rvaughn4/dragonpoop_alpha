
#include "openglx_1o5_renderer_model_man.h"
#include "openglx_1o5_renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_man::openglx_1o5_renderer_model_man( core *c, openglx_1o5_renderer *r, dptaskpool_writelock *tp ) : renderer_model_man( c, r, tp )
    {
        this->r = r;
    }
    
    //dtor
    openglx_1o5_renderer_model_man::~openglx_1o5_renderer_model_man( void )
    {
        this->deleteModels();
    }
    
    //generate renderer model
    renderer_model *openglx_1o5_renderer_model_man::genModel( model_writelock *ml )
    {
        return new openglx_1o5_renderer_model( ml, this->r );
    }
    
};
