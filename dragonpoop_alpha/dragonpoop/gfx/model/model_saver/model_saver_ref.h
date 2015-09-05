
#ifndef dragonpoop_model_saver_ref_h
#define dragonpoop_model_saver_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{
    
    class model_saver;
    
    class model_saver_ref : public shared_obj_ref
    {
        
    private:
        
        model_saver *t;
        
    protected:
        
        //ctor
        model_saver_ref( model_saver *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~model_saver_ref( void );
        
        friend class model_saver;
    };
    
};

#endif