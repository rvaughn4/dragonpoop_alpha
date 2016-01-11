
#ifndef dragonpoop_input_passer_ref_h
#define dragonpoop_input_passer_ref_h

#include "../../../core/shared_obj/shared_obj_ref.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class input_passer;

    class input_passer_ref : public shared_obj_ref
    {

    private:

        input_passer *t;
        std::shared_ptr<std::atomic<uint64_t>> tm;

    protected:

        //ctor
        input_passer_ref( input_passer *p, std::shared_ptr<shared_obj_refkernal> *k, std::shared_ptr<std::atomic<uint64_t>> tm );

    public:

        //dtor
        virtual ~input_passer_ref( void );
        //get position time
        uint64_t getTime( void );

        friend class input_passer;
    };

};

#endif
