
#ifndef dragonpoop_dpland_man_ref_h
#define dragonpoop_dpland_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class dpland_man;
    
    class dpland_man_ref : public shared_obj_ref
    {
        
    private:
        
        dpland_man *t;
        
    protected:
        
        //ctor
        dpland_man_ref( dpland_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~dpland_man_ref( void );
        
        friend class dpland_man;
    };
    
};

#endif