
#ifndef dragonpoop_renderer_model_instance_group_h
#define dragonpoop_renderer_model_instance_group_h

#include "../../../../gfx/model/model_component/model_components.h"
#include "../../../../gfx/dpvertex/dpvertexindex_buffer.h"
#include <vector>

namespace dragonpoop
{
 
    class model_instance_group;
    class model_instance_writelock;
    class dpthread_lock;
    class renderer_model_instance_readlock;
    
    class renderer_model_instance_group : public model_component
    {
        
    private:
        
        dpid m_id;
        dpvertexindex_buffer vb;
        dpvertex_buffer tvb;
        dpindex_buffer tib;

    protected:
        
    public:
        
        //ctor
        renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //dtor
        virtual ~renderer_model_instance_group( void );
        //set material id
        void setMaterialId( dpid id );
        //get material id
        dpid getMaterialId( void );
        //sync with group
        virtual void sync( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //sync with group
        virtual void animate( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //returns vertex buffer
        dpvertexindex_buffer *getVertexBuffer( void );
        //returns transformed vertex buffer
        dpvertex_buffer *getTransformedBuffer( renderer_model_instance_readlock *m, dpindex_buffer **li );

    };
    
};

#endif