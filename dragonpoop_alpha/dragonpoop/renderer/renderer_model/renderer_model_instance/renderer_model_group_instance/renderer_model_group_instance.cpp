
#include "renderer_model_group_instance.h"
#include "../../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"
#include "../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../renderer_model_instance_readlock.h"


namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_group::renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd ) : model_component( g->getId(), model_component_type_group )
    {
        this->sync( ml, g, thd );
    }
    
    //dtor
    renderer_model_instance_group::~renderer_model_instance_group( void )
    {
        
    }
    
    //set material id
    void renderer_model_instance_group::setMaterialId( dpid id )
    {
        this->m_id = id;
    }
    
    //get material id
    dpid renderer_model_instance_group::getMaterialId( void )
    {
        return this->m_id;
    }

    //sync with group
    void renderer_model_instance_group::sync( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
        this->m_id = g->getMaterialId();
        this->vb.clear();
        ml->fillVertexBuffer( g->getId(), &this->vb );
    }
    
    //sync with group
    void renderer_model_instance_group::animate( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd )
    {
    }
    
    //returns vertex buffer
    dpvertexindex_buffer *renderer_model_instance_group::getVertexBuffer( void )
    {
        return &this->vb;
    }
    
    //returns transformed vertex buffer
    dpvertex_buffer *renderer_model_instance_group::getTransformedBuffer( renderer_model_instance_readlock *m, dpindex_buffer **li )
    {
        unsigned int i, e;
        dpvertex *v, *bv;
        
        this->tib.copy( this->vb.getIB() );
        this->tvb.copy( this->vb.getVB() );
        
        e = this->tvb.getSize();
        bv = this->tvb.getBuffer();
        for( i = 0; i < e && bv; i++ )
        {
            v = &bv[ i ];
            m->transform( v );
        }
        if( li )
            *li = &this->tib;

        return &this->tvb;
    }
    
};
