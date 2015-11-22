
#ifndef dragonpoop_dpposition_share_h
#define dragonpoop_dpposition_share_h

#include "../../core/shared_obj/shared_obj.h"
#include "dpposition.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dpposition_share : public shared_obj
    {

    private:

        dpposition pos;
        std::shared_ptr<std::atomic<uint64_t>> t;

    protected:

        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //set camera position
        void setPosition( dpposition *p );
        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );

    public:

        //ctor
        dpposition_share( dpmutex_master *mm );
        //dtor
        virtual ~dpposition_share( void );


        friend class dpposition_share_writelock;
        friend class dpposition_share_readlock;
        friend class dpposition_share_ref;

    };

};

#endif
