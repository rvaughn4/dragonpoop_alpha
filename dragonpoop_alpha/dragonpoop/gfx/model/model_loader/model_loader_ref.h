
#ifndef dragonpoop_model_loader_ref_h
#define dragonpoop_model_loader_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class model_loader;
    
    class model_loader_ref : public shared_obj_ref
    {
        
    private:
        
        model_loader *t;
        
    protected:
        
        //ctor
        model_loader_ref( model_loader *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~model_loader_ref( void );
        
        friend class model_loader;
    };
    
};

#endif