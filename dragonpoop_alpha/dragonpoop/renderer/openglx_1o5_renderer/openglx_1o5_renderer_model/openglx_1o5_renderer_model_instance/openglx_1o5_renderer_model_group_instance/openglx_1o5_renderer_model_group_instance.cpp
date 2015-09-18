
#include "openglx_1o5_renderer_model_group_instance.h"
#include "../../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_group::openglx_1o5_renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd ) : renderer_model_instance_group( ml, g, thd )
    {
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_group::~openglx_1o5_renderer_model_instance_group( void )
    {
    }
    
    //sync with group
    void openglx_1o5_renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
        this->renderer_model_instance_group::sync( ml, g, thd );
    }
    
};
