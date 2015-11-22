
#ifndef dragonpoop_dpposition_share_readlock_h
#define dragonpoop_dpposition_share_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class dpposition_share;
    class dpposition;

    class dpposition_share_readlock : public shared_obj_readlock
    {

    private:

        dpposition_share *t;

    protected:

        //ctor
        dpposition_share_readlock( dpposition_share *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpposition_share_readlock( void );

    public:

        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );

        friend class dpposition_share;
    };

};

#endif
