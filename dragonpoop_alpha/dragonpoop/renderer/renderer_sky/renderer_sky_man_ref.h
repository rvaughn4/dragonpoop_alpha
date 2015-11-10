
#ifndef dragonpoop_renderer_sky_man_ref_h
#define dragonpoop_renderer_sky_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class renderer_sky_man;
    
    class renderer_sky_man_ref : public shared_obj_ref
    {
        
    private:
        
        renderer_sky_man *t;
        
    protected:
        
        //ctor
        renderer_sky_man_ref( renderer_sky_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~renderer_sky_man_ref( void );
        
        friend class renderer_sky_man;
    };
    
};

#endif