
#include "dpactor_animate_state.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_animate_state::dpactor_animate_state( dpactor *a ) : dpactor_state( a )
    {
        this->a = a;
    }
    
    //dtor
    dpactor_animate_state::~dpactor_animate_state( void )
    {
        
    }
    
    //run state
    void dpactor_animate_state::run( dpthread_lock *thd, dpactor *a, dpactor_state **next_state )
    {
        return this->run( thd, a, (dpactor_animate_state **)next_state );
    }
    
    //run state
    void dpactor_animate_state::run( dpthread_lock *thd, dpactor *a, dpactor_animate_state **next_state )
    {
        *next_state = 0;
    }
    
    //stop all animations
    void dpactor_animate_state::stopAnimations( void )
    {
        this->a->stopAnimations();
    }
    
    //play animation
    void dpactor_animate_state::playAnimation( const char *cname, float spd, bool doRepeat )
    {
        this->a->playAnimation( cname, spd, doRepeat );
    }
    
    //get position
    void dpactor_animate_state::getPosition( dpposition *p )
    {
        this->a->getPosition( p );
    }
    
    //returns true if animation is playing
    bool dpactor_animate_state::isPlaying( const char *cname )
    {
        return this->a->isPlaying( cname );
    }
    
};
