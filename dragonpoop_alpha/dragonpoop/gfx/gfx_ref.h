
#ifndef dragonpoop_gfx_ref_h
#define dragonpoop_gfx_ref_h

#include "../core/shared_obj/shared_obj_ref.h"

namespace dragonpoop
{

    class gfx;

    class gfx_ref : public shared_obj_ref
    {

    private:

        gfx *t;

    protected:

        //ctor
        gfx_ref( gfx *p, std::shared_ptr<shared_obj_refkernal> *k );

    public:

        //dtor
        virtual ~gfx_ref( void );

        friend class gfx;
    };
    
};

#endif