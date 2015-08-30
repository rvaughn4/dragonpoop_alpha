
#ifndef dragonpoop_model_instance_group_h
#define dragonpoop_model_instance_group_h

#include "../../model_component/model_components.h"
#include "../../model_group/model_group.h"
#include "../../../dpvertex/dpvertexindex_buffer.h"

namespace dragonpoop
{
    
    class dpvertexindex_buffer;
    
    class model_instance_group : public model_component
    {
        
    private:
        
        dpid m_id;
        dpvertexindex_buffer vb;
        
    protected:
        
    public:
        
        //ctor
        model_instance_group( model_group *g );
        //dtor
        virtual ~model_instance_group( void );
        //set material id
        void setMaterialId( dpid id );
        //get material id
        dpid getMaterialId( void );
        //get vertex buffer
        dpvertexindex_buffer *getVertexBuffer();

    };
    
};

#endif