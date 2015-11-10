
#ifndef dragonpoop_renderer_commandlist_passer_ref_h
#define dragonpoop_renderer_commandlist_passer_ref_h

#include "../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class renderer_commandlist_passer;
    
    class renderer_commandlist_passer_ref : public shared_obj_ref
    {
        
    private:
        
        renderer_commandlist_passer *t;
        
    protected:
        
        //ctor
        renderer_commandlist_passer_ref( renderer_commandlist_passer *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~renderer_commandlist_passer_ref( void );
        
        friend class renderer_commandlist_passer;
        friend class renderer;
        friend class renderer_gui_man;
        friend class renderer_model_man;
        friend class renderer_land_man;
        friend class renderer_sky_man;
    };
    
};

#endif