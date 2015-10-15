
#ifndef dragonpoop_model_man_ref_h
#define dragonpoop_model_man_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class model_man;
    
    class model_man_ref : public shared_obj_ref
    {
        
    private:
        
        model_man *t;
        
    protected:
        
        //ctor
        model_man_ref( model_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~model_man_ref( void );
        
        friend class model_man;
    };
    
};

#endif