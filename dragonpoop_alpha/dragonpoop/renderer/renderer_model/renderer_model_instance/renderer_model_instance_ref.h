
#ifndef dragonpoop_renderer_model_instance_ref_h
#define dragonpoop_renderer_model_instance_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class renderer_model_instance;
    
    class renderer_model_instance_ref : public shared_obj_ref
    {
        
    private:
        
        renderer_model_instance *t;
        
    protected:
        
        //ctor
        renderer_model_instance_ref( renderer_model_instance *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~renderer_model_instance_ref( void );
        
        friend class renderer_model_instance;
    };
    
};

#endif