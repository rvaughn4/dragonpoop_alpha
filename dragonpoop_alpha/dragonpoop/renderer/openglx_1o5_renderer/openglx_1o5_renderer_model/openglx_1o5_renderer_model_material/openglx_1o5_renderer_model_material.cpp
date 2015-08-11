
#include "openglx_1o5_renderer_model_material.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_material::openglx_1o5_renderer_model_material( model_writelock *ml, model_material *m ) : renderer_model_material( ml, m )
    {
        this->sync( ml, m );
    }
    
    //dtor
    openglx_1o5_renderer_model_material::~openglx_1o5_renderer_model_material( void )
    {
        
    }
    
    //sync with group
    void openglx_1o5_renderer_model_material::sync( model_writelock *ml, model_material *m )
    {
        this->renderer_model_material::sync( ml, m );
    }

};
