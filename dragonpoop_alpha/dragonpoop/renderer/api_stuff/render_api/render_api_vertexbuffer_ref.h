
#ifndef dragonpoop_render_api_vertexbuffer_ref_h
#define dragonpoop_render_api_vertexbuffer_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class render_api_vertexbuffer;
    
    class render_api_vertexbuffer_ref : public shared_obj_ref
    {
        
    private:
        
        render_api_vertexbuffer *t;
        
    protected:
        
        //ctor
        render_api_vertexbuffer_ref( render_api_vertexbuffer *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~render_api_vertexbuffer_ref( void );
        
        friend class render_api_vertexbuffer;
    };
    
};

#endif