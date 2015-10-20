
#ifndef dragonpoop_render_api_ref_h
#define dragonpoop_render_api_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class render_api;
    
    class render_api_ref : public shared_obj_ref
    {
        
    private:
        
        render_api *t;
        
    protected:
        
        //ctor
        render_api_ref( render_api *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~render_api_ref( void );
        
        friend class render_api;
    };
    
};

#endif