
#ifndef dragonpoop_render_api_indexbuffer_ref_h
#define dragonpoop_render_api_indexbuffer_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class render_api_indexbuffer;
    
    class render_api_indexbuffer_ref : public shared_obj_ref
    {
        
    private:
        
        render_api_indexbuffer *t;
        
    protected:
        
        //ctor
        render_api_indexbuffer_ref( render_api_indexbuffer *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~render_api_indexbuffer_ref( void );
        
        friend class render_api_indexbuffer;
    };
    
};

#endif