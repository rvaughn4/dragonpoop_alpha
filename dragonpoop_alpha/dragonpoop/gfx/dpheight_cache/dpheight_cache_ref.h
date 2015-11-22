
#ifndef dragonpoop_dpheight_cache_ref_h
#define dragonpoop_dpheight_cache_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dpheight_cache;

    class dpheight_cache_ref : public shared_obj_ref
    {

    private:

        dpheight_cache *t;
        std::shared_ptr<std::atomic<uint64_t>> tm;

    protected:

        //ctor
        dpheight_cache_ref( dpheight_cache *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm );

    public:

        //dtor
        virtual ~dpheight_cache_ref( void );
        //get position time
        uint64_t getTime( void );

        friend class dpheight_cache;
    };

};

#endif
