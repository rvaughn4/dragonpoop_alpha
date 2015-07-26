
#include "shared_obj_refkernal.h"
#include "../dpspinlock/dpsponlock.h"

namespace dragonpoop
{

    //ctor
    shared_obj_refkernal::shared_obj_refkernal( void )
    {
        this->bIsOrigLinked = 1;
        this->bIsRefLinked = 1;
        this->slk = new dpspinlock();
    }

    //dtor
    shared_obj_refkernal::~shared_obj_refkernal( void )
    {
        delete this->slk;
    }

    //returns true if linked
    bool shared_obj_refkernal::isLinked( void )
    {
        return this->bIsOrigLinked && this->bIsRefLinked;
    }

    //unlink original
    void shared_obj_refkernal::unlinkOriginal( void )
    {
        this->bIsOrigLinked = 0;
    }

    //unlink ref
    void shared_obj_refkernal::unlinkRef( void )
    {
        this->bIsRefLinked = 0;
    }

    //lock
    void shared_obj_refkernal::lock( void )
    {
        this->slk->lock();
    }

    //unlock
    void shared_obj_refkernal::unlock( void )
    {
        this->slk->unlock();
    }

};
