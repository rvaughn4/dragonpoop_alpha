
#ifndef dragonpoop_dpactor_ref_h
#define dragonpoop_dpactor_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class dpactor;
    
    class dpactor_ref : public shared_obj_ref
    {
        
    private:
        
        dpactor *t;
        
    protected:
        
        //ctor
        dpactor_ref( dpactor *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~dpactor_ref( void );
        
        friend class dpactor;
    };
    
};

#endif