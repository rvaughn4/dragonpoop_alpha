
#ifndef dragonpoop_dpheight_cache_writelock_h
#define dragonpoop_dpheight_cache_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class dpheight_cache;
    class dpposition;

    class dpheight_cache_writelock : public shared_obj_writelock
    {

    private:

        dpheight_cache *t;

    protected:

        //ctor
        dpheight_cache_writelock( dpheight_cache *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpheight_cache_writelock( void );

    public:

        //set camera position
        void setPosition( dpposition *p );
        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );

        friend class dpheight_cache;
    };

};

#endif
