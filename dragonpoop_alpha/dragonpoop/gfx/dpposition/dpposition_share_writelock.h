
#ifndef dragonpoop_dpposition_share_writelock_h
#define dragonpoop_dpposition_share_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class dpposition_share;
    class dpposition;

    class dpposition_share_writelock : public shared_obj_writelock
    {

    private:

        dpposition_share *t;

    protected:

        //ctor
        dpposition_share_writelock( dpposition_share *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpposition_share_writelock( void );

    public:

        //set camera position
        void setPosition( dpposition *p );
        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );

        friend class dpposition_share;
    };

};

#endif
