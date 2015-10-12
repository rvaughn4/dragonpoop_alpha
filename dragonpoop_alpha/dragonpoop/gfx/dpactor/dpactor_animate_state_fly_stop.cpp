
#include "dpactor_animate_state_fly_stop.h"
#include "dpactor_animate_state_fly.h"
#include "dpactor_animate_state_fly_start.h"
#include "dpactor_animate_state_idle.h"
#include "dpactor_animate_state_walk.h"
#include "dpactor.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_animate_state_fly_stop::dpactor_animate_state_fly_stop( dpactor *a ) : dpactor_animate_state( a )
    {
        this->stopAnimations();
        this->playAnimation( "fly_stop", 0.25f, 0 );
    }
    
    //dtor
    dpactor_animate_state_fly_stop::~dpactor_animate_state_fly_stop( void )
    {
        
    }
    
    //run state
    void dpactor_animate_state_fly_stop::run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state )
    {
        uint64_t t;
        dpposition p;
        dpposition_inner pi;
        
        this->getPosition( &p );
        p.getData( &pi );
        
        if( pi.end.whole.y > pi.start.whole.y )
        {
            *next_state = new dpactor_animate_state_fly_start( a );
            return;
        }
        
        *next_state = 0;
        if( this->isPlaying( "fly_stop" ) )
            return;

        if( p.isMoving( t ) )
        {
            *next_state = new dpactor_animate_state_walk( a );
            return;
        }
        
        *next_state = new dpactor_animate_state_idle( a );
    }
    
};
