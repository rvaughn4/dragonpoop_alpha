
#ifndef dragonpoop_model_instance_ref_h
#define dragonpoop_model_instance_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class model_instance;
    
    class model_instance_ref : public shared_obj_ref
    {
        
    private:
        
        model_instance *t;
        
    protected:
        
        //ctor
        model_instance_ref( model_instance *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~model_instance_ref( void );
        
        friend class model_instance;
    };
    
};

#endif