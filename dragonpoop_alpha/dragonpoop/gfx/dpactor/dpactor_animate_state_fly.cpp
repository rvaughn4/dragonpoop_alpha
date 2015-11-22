
#include "dpactor_animate_state_fly.h"
#include "dpactor_animate_state_fly_idle.h"
#include "dpactor_animate_state_walk.h"
#include "dpactor.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{

    //ctor
    dpactor_animate_state_fly::dpactor_animate_state_fly( dpactor *a ) : dpactor_animate_state( a )
    {
        this->t_start = 0;
        this->stopAnimations();
        this->playAnimation( "fly", 1, 1 );
    }

    //dtor
    dpactor_animate_state_fly::~dpactor_animate_state_fly( void )
    {

    }

    //run state
    void dpactor_animate_state_fly::run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state )
    {
        uint64_t t;
        dpposition p;
        dpposition_inner pi;

        t = thd->getTicks();
        if( !this->t_start )
            this->t_start = t;
        if( t - this->t_start > 20000 )
        {
            *next_state = new dpactor_animate_state_fly( a );
            return;
        }

        this->getPosition( &p );
        p.getData( &pi );

        if( !p.isMoving( t ) )
        {
            *next_state = new dpactor_animate_state_fly_idle( a );
            return;
        }
        if( abs(pi.end.y) <= 0.1f )
        {
            *next_state = new dpactor_animate_state_walk( a );
            return;
        }

        *next_state = 0;
    }

};
