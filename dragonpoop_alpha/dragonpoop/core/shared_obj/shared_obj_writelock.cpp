
#include "shared_obj_writelock.h"
#include "../dpmutex/dpmutex_writelock.h"

namespace dragonpoop
{

    //ctor
    shared_obj_writelock::shared_obj_writelock( shared_obj_interface *p, dpmutex_writelock *l ) : shared_obj_lock( p, l )
    {
        this->p = p;
        this->l = l;
    }

    //dtor
    shared_obj_writelock::~shared_obj_writelock( void )
    {
        
    }

    //return a new ref
    shared_obj_ref *shared_obj_writelock::getRef( void )
    {
        return this->shared_obj_lock::getRef();
    }

    //unlink ref
    void shared_obj_writelock::unlink( void )
    {
        this->shared_obj_lock::unlink();
    }

};
