
#ifndef dragonpoop_dptaskpool_logger_ref_h
#define dragonpoop_dptaskpool_logger_ref_h

#include "../../core/shared_obj/shared_obj_ref.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dptaskpool_logger;

    class dptaskpool_logger_ref : public shared_obj_ref
    {

    private:

        dptaskpool_logger *t;
        std::shared_ptr<std::atomic<uint64_t>> tm;

    protected:

        //ctor
        dptaskpool_logger_ref( dptaskpool_logger *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm );

    public:

        //dtor
        virtual ~dptaskpool_logger_ref( void );
        //get position time
        uint64_t getTime( void );

        friend class dptaskpool_logger;
    };

};

#endif
