
#ifndef dragonpoop_model_loader_man_ref_h
#define dragonpoop_model_loader_man_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class model_loader_man;
    
    class model_loader_man_ref : public shared_obj_ref
    {
        
    private:
        
        model_loader_man *t;
        
    protected:
        
        //ctor
        model_loader_man_ref( model_loader_man *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~model_loader_man_ref( void );
        
        friend class model_loader_man;
    };
    
};

#endif