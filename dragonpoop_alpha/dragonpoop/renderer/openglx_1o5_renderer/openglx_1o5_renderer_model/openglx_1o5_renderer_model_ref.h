
#ifndef dragonpoop_openglx_1o5_renderer_model_ref_h
#define dragonpoop_openglx_1o5_renderer_model_ref_h

#include "../../renderer_model/renderer_model_ref.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model;
    
    class openglx_1o5_renderer_model_ref : public renderer_model_ref
    {
        
    private:
        
        openglx_1o5_renderer_model *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_model_ref( openglx_1o5_renderer_model *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~openglx_1o5_renderer_model_ref( void );
        
        friend class openglx_1o5_renderer_model;
    };
    
};

#endif