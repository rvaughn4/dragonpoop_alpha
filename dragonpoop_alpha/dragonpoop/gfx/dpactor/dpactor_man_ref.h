
#ifndef dragonpoop_dpactor_man_ref_h
#define dragonpoop_dpactor_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class dpactor_man;
    
    class dpactor_man_ref : public shared_obj_ref
    {
        
    private:
        
        dpactor_man *t;
        
    protected:
        
        //ctor
        dpactor_man_ref( dpactor_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~dpactor_man_ref( void );
        
        friend class dpactor_man;
    };
    
};

#endif