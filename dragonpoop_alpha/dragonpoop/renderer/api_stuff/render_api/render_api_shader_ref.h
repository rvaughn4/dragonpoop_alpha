
#ifndef dragonpoop_render_api_shader_ref_h
#define dragonpoop_render_api_shader_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class render_api_shader;
    
    class render_api_shader_ref : public shared_obj_ref
    {
        
    private:
        
        render_api_shader *t;
        
    protected:
        
        //ctor
        render_api_shader_ref( render_api_shader *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~render_api_shader_ref( void );
        
        friend class render_api_shader;
    };
    
};

#endif