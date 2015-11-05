
#ifndef dragonpoop_renderer_land_man_ref_h
#define dragonpoop_renderer_land_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class renderer_land_man;
    
    class renderer_land_man_ref : public shared_obj_ref
    {
        
    private:
        
        renderer_land_man *t;
        
    protected:
        
        //ctor
        renderer_land_man_ref( renderer_land_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~renderer_land_man_ref( void );
        
        friend class renderer_land_man;
    };
    
};

#endif