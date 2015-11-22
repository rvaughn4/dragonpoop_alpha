
#ifndef dragonpoop_dpheight_cache_readlock_h
#define dragonpoop_dpheight_cache_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class dpheight_cache;
    class dpposition;

    class dpheight_cache_readlock : public shared_obj_readlock
    {

    private:

        dpheight_cache *t;

    protected:

        //ctor
        dpheight_cache_readlock( dpheight_cache *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpheight_cache_readlock( void );

    public:

        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );

        friend class dpheight_cache;
    };

};

#endif
