
#ifndef dragonpoop_openglx_1o5_renderer_model_instance_ref_h
#define dragonpoop_openglx_1o5_renderer_model_instance_ref_h

#include "../../../renderer_model/renderer_model_instance/renderer_model_instance_ref.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model_instance;
    
    class openglx_1o5_renderer_model_instance_ref : public renderer_model_instance_ref
    {
        
    private:
        
        openglx_1o5_renderer_model_instance *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_model_instance_ref( openglx_1o5_renderer_model_instance *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~openglx_1o5_renderer_model_instance_ref( void );
        
        friend class openglx_1o5_renderer_model_instance;
    };
    
};

#endif