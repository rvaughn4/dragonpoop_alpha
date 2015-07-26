
#ifndef dragonpoop_dptask_ref_h
#define dragonpoop_dptask_ref_h

#include "../shared_obj/shared_obj_ref.h"

namespace dragonpoop
{

    class dptask;

    class dptask_ref : public shared_obj_ref
    {

    private:

        dptask *t;

    protected:

        //ctor
        dptask_ref( dptask *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~dptask_ref( void );

        friend class dptask;
    };
    
};

#endif