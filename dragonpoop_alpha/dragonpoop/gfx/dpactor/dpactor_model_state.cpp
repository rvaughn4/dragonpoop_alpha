
#include "dpactor_model_state.h"
#include "dpactor.h"

namespace dragonpoop
{
    
    //ctor
    dpactor_model_state::dpactor_model_state( dpactor *a ) : dpactor_state( a )
    {
        this->a = a;
    }
    
    //dtor
    dpactor_model_state::~dpactor_model_state( void )
    {
        
    }
    
    //run state
    void dpactor_model_state::run( dpthread_lock *thd, dpactor *a, dpactor_state **next_state )
    {
        return this->run( thd, a, (dpactor_model_state **)next_state );
    }
    
    //run state
    void dpactor_model_state::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        *next_state = 0;
    }
    
    //return camera distance
    float dpactor_model_state::getDistance( void )
    {
        return this->a->getCameraDistance();
    }
    
    //returns true if low is created
    bool dpactor_model_state::isLow( void )
    {
        return this->a->models.low.mi != 0;
    }
    
    //load low model
    bool dpactor_model_state::loadLow( model_loader_ref **ldr )
    {
        return this->a->loadLow( ldr );
    }
    
    //make low model instance
    bool dpactor_model_state::makeLow( dpthread_lock *thd )
    {
        return this->a->makeLow( thd );
    }
    
    //destroy low model
    void dpactor_model_state::killLow( void )
    {
        this->a->killLow();
    }
    
    //returns true if med is created
    bool dpactor_model_state::isMed( void )
    {
        return this->a->models.med.mi != 0;
    }
    
    //load med model
    bool dpactor_model_state::loadMed( model_loader_ref **ldr )
    {
        return this->a->loadMed( ldr );
    }
    
    //make med model instance
    bool dpactor_model_state::makeMed( dpthread_lock *thd )
    {
        return this->a->makeMed( thd );
    }
    
    //destroy low model
    void dpactor_model_state::killMed( void )
    {
        this->a->killMed();
    }

    //returns true if high is created
    bool dpactor_model_state::isHigh( void )
    {
        return this->a->models.high.mi != 0;
    }
    
    //load low model
    bool dpactor_model_state::loadHigh( model_loader_ref **ldr )
    {
        return this->a->loadHigh( ldr );
    }
    
    //make low model instance
    bool dpactor_model_state::makeHigh( dpthread_lock *thd )
    {
        return this->a->makeHigh( thd );
    }
    
    //destroy low model
    void dpactor_model_state::killHigh( void )
    {
        this->a->killHigh();
    }
    
};
