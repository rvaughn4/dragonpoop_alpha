
#ifndef dragonpoop_renderer_model_ref_h
#define dragonpoop_renderer_model_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class renderer_model;
    
    class renderer_model_ref : public shared_obj_ref
    {
        
    private:
        
        renderer_model *t;
        
    protected:
        
        //ctor
        renderer_model_ref( renderer_model *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~renderer_model_ref( void );
        
        friend class renderer_model;
    };
    
};

#endif