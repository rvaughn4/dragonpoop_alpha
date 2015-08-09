
#ifndef dragonpoop_renderer_model_instance_group_h
#define dragonpoop_renderer_model_instance_group_h

#include "../../../../gfx/model/model_component/model_components.h"

namespace dragonpoop
{
 
    class model_instance_group;
    class model_instance_writelock;
    
    class renderer_model_instance_group : public model_component
    {
        
    private:
        
        dpid m_id;
        
    protected:
        
    public:
        
        //ctor
        renderer_model_instance_group( model_instance_writelock *ml, model_instance_group *g );
        //dtor
        virtual ~renderer_model_instance_group( void );
        //set material id
        void setMaterialId( dpid id );
        //get material id
        dpid getMaterialId( void );
        //sync with group
        virtual void sync( model_instance_writelock *ml, model_instance_group *g );
        
    };
    
};

#endif