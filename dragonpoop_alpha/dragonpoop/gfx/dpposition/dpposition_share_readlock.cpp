
#include "dpposition_share_readlock.h"
#include "dpposition_share.h"

namespace dragonpoop
{

    //ctor
    dpposition_share_readlock::dpposition_share_readlock( dpposition_share *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    dpposition_share_readlock::~dpposition_share_readlock( void )
    {

    }

    //get camera position
    dpposition *dpposition_share_readlock::getPosition( void )
    {
        return this->t->getPosition();
    }

    //get position time
    uint64_t dpposition_share_readlock::getTime( void )
    {
        return this->t->getTime();
    }

};
