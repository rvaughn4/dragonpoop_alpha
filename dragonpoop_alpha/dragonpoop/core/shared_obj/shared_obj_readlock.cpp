
#include "shared_obj_readlock.h"
#include "../dpmutex/dpmutex_readlock.h"

namespace dragonpoop
{

    //ctor
    shared_obj_readlock::shared_obj_readlock( shared_obj_interface *p, dpmutex_readlock *l ) : shared_obj_lock( p, l )
    {
        this->p = p;
        this->l = l;
    }

    //dtor
    shared_obj_readlock::~shared_obj_readlock( void )
    {

    }

};
