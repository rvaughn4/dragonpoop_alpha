
#include "dpposition_share_writelock.h"
#include "dpposition_share.h"

namespace dragonpoop
{

    //ctor
    dpposition_share_writelock::dpposition_share_writelock( dpposition_share *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    dpposition_share_writelock::~dpposition_share_writelock( void )
    {

    }

    //set camera position
    void dpposition_share_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }

    //get camera position
    dpposition *dpposition_share_writelock::getPosition( void )
    {
        return this->t->getPosition();
    }

    //get position time
    uint64_t dpposition_share_writelock::getTime( void )
    {
        return this->t->getTime();
    }

};
