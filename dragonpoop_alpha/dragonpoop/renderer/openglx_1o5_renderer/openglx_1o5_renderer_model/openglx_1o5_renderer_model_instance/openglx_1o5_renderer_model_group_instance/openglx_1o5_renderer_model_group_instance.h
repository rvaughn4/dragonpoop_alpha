
#ifndef dragonpoop_openglx_1o5_renderer_model_instance_group_h
#define dragonpoop_openglx_1o5_renderer_model_instance_group_h

#include "../../../../renderer_model/renderer_model_instance/renderer_model_group_instance/renderer_model_group_instance.h"
#include "../../../../../gfx/dpvertex/dpvertexindex_buffer.h"

namespace dragonpoop
{
    
    class openglx_1o5_model_instance_group;
    class dpthread_lock;
    
    class openglx_1o5_renderer_model_instance_group : public renderer_model_instance_group
    {
        
    private:
        
        dpvertexindex_buffer *vb;
        
    protected:
        
    public:
        
        //ctor
        openglx_1o5_renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //dtor
        virtual ~openglx_1o5_renderer_model_instance_group( void );
        //sync with group
        virtual void sync( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //returns vertex buffer
        dpvertexindex_buffer *getVertexBuffer( void );
        
    };
    
};

#endif