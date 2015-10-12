
#include "dpactor_animate_state_walk.h"
#include "dpactor_animate_state_idle.h"
#include "dpactor_animate_state_fly_start.h"
#include "dpactor_animate_state_fly_stop.h"
#include "dpactor.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_animate_state_walk::dpactor_animate_state_walk( dpactor *a ) : dpactor_animate_state( a )
    {
        this->t_start = 0;
        this->stopAnimations();
        this->playAnimation( "walk", 3, 1 );
    }
    
    //dtor
    dpactor_animate_state_walk::~dpactor_animate_state_walk( void )
    {
        
    }
    
    //run state
    void dpactor_animate_state_walk::run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state )
    {
        uint64_t t;
        dpposition p;
        dpposition_inner pi;
        
        t = thd->getTicks();
        if( !this->t_start )
            this->t_start = t;
        if( t - this->t_start > 20000 )
        {
            *next_state = new dpactor_animate_state_walk( a );
            return;
        }
        
        this->getPosition( &p );
        p.getData( &pi );
      
        if( !p.isMoving( t ) )
        {
            *next_state = new dpactor_animate_state_idle( a );
            return;
        }
        if( pi.end.whole.y > pi.start.whole.y )
        {
            *next_state = new dpactor_animate_state_fly_start( a );
            return;
        }
        if( pi.end.whole.y < pi.start.whole.y )
        {
            *next_state = new dpactor_animate_state_fly_stop( a );
            return;
        }
                
        *next_state = 0;
    }
    
};
