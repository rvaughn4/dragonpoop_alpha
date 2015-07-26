
#ifndef dragonpoop_model_ref_h
#define dragonpoop_model_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{

    class model;

    class model_ref : public shared_obj_ref
    {

    private:

        model *t;

    protected:

        //ctor
        model_ref( model *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~model_ref( void );
        
        friend class model;
    };
    
};

#endif