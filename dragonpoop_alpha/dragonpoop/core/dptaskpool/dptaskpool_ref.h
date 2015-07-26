
#ifndef dragonpoop_dptaskpool_ref_h
#define dragonpoop_dptaskpool_ref_h

#include "../shared_obj/shared_obj_ref.h"

namespace dragonpoop
{

    class dptaskpool;

    class dptaskpool_ref : public shared_obj_ref
    {

    private:

        dptaskpool *t;

    protected:

        //ctor
        dptaskpool_ref( dptaskpool *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~dptaskpool_ref( void );
        
        friend class dptaskpool;
    };
    
};

#endif