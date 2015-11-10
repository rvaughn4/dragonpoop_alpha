
#ifndef dragonpoop_dpsky_man_ref_h
#define dragonpoop_dpsky_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class dpsky_man;
    
    class dpsky_man_ref : public shared_obj_ref
    {
        
    private:
        
        dpsky_man *t;
        
    protected:
        
        //ctor
        dpsky_man_ref( dpsky_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~dpsky_man_ref( void );
        
        friend class dpsky_man;
    };
    
};

#endif