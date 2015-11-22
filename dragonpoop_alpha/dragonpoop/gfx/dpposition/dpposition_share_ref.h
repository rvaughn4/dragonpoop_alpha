
#ifndef dragonpoop_dpposition_share_ref_h
#define dragonpoop_dpposition_share_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dpposition_share;

    class dpposition_share_ref : public shared_obj_ref
    {

    private:

        dpposition_share *t;
        std::shared_ptr<std::atomic<uint64_t>> tm;

    protected:

        //ctor
        dpposition_share_ref( dpposition_share *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm );

    public:

        //dtor
        virtual ~dpposition_share_ref( void );
        //get position time
        uint64_t getTime( void );

        friend class dpposition_share;
    };

};

#endif
