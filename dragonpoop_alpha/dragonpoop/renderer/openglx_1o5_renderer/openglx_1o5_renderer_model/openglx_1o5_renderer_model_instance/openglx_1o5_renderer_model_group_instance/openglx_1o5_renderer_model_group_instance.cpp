
#include "openglx_1o5_renderer_model_group_instance.h"
#include "../../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_group::openglx_1o5_renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g ) : renderer_model_instance_group( ml, g )
    {
        this->vb = new dpvertexindex_buffer();
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_group::~openglx_1o5_renderer_model_instance_group( void )
    {
        delete this->vb;
    }
    
    //sync with group
    void openglx_1o5_renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g )
    {
        this->renderer_model_instance_group::sync( ml, g );
        this->vb->clear();
        ml->fillVertexBuffer( g->getId(), this->vb );
    }

    //returns vertex buffer
    dpvertexindex_buffer *openglx_1o5_renderer_model_instance_group::getVertexBuffer( void )
    {
        return this->vb;
    }
    
};
