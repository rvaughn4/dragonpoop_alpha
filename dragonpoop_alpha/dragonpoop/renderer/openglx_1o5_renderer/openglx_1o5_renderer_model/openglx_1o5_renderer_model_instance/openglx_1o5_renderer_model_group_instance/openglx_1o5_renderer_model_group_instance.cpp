
#include "openglx_1o5_renderer_model_group_instance.h"
#include "../../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_model_instance_group::openglx_1o5_renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g ) : renderer_model_instance_group( ml, g )
    {
        this->vb = new dpvertexindex_buffer();
        this->vbsmooth = new dpvertex_buffer();
        this->f_ratio = 0;
    }
    
    //dtor
    openglx_1o5_renderer_model_instance_group::~openglx_1o5_renderer_model_instance_group( void )
    {
        delete this->vb;
        delete this->vbsmooth;
    }
    
    //sync with group
    void openglx_1o5_renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g )
    {
        dpvertex *b;
        unsigned int sn, so;
        this->renderer_model_instance_group::sync( ml, g );
        this->vb->clear();
        ml->fillVertexBuffer( g->getId(), this->vb );
        
        so = this->vbsmooth->getSize();
        b = this->vb->getVertexBuffer( &sn );
        while( so < sn )
        {
            this->vbsmooth->addVertex( &b[ 0 ] );
            so++;
        }
    }

    //returns vertex buffer
    dpvertexindex_buffer *openglx_1o5_renderer_model_instance_group::getVertexBuffer( void )
    {
        return this->vb;
    }
    
    //returns smoothing buffer
    dpvertex_buffer *openglx_1o5_renderer_model_instance_group::getSmoothBuffer( void )
    {
        return this->vbsmooth;
    }
    
    //returns smooth time
    uint64_t openglx_1o5_renderer_model_instance_group::getSmoothTime( void )
    {
        return this->last_smooth_time;
    }
    
    //sets smooth time
    void openglx_1o5_renderer_model_instance_group::setSmoothTime( uint64_t t )
    {
        this->last_smooth_time = t;
    }
    
    //sets smooth ratio
    void openglx_1o5_renderer_model_instance_group::setSmoothRatio( float f )
    {
        this->f_ratio = f;
    }
    
    //gets smooth ratio
    float openglx_1o5_renderer_model_instance_group::getSmoothRatio( void )
    {
        return this->f_ratio;
    }
    
};
