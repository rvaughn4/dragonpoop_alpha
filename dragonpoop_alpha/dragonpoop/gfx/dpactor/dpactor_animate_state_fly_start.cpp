
#include "dpactor_animate_state_fly_start.h"
#include "dpactor_animate_state_fly.h"
#include "dpactor_animate_state_fly_stop.h"
#include "dpactor_animate_state_fly_idle.h"
#include "dpactor.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_animate_state_fly_start::dpactor_animate_state_fly_start( dpactor *a ) : dpactor_animate_state( a )
    {
        this->stopAnimations();
        this->playAnimation( "fly_start", 0.25f, 0 );
    }
    
    //dtor
    dpactor_animate_state_fly_start::~dpactor_animate_state_fly_start( void )
    {
        
    }
    
    //run state
    void dpactor_animate_state_fly_start::run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state )
    {
        uint64_t t;
        dpposition p;
        dpposition_inner pi;
        
        this->getPosition( &p );
        p.getData( &pi );
        
        if( p.isMoving( t ) && pi.end.whole.y < pi.start.whole.y )
        {
            *next_state = new dpactor_animate_state_fly_stop( a );
            return;
        }

        *next_state = 0;
        if( this->isPlaying( "fly_start" ) )
            return;
        
        if( p.isMoving( t ) )
        {
            *next_state = new dpactor_animate_state_fly( a );
            return;
        }
        
        *next_state = new dpactor_animate_state_fly_idle( a );
    }
    
};
