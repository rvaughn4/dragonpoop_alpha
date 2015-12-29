
#include "dpactor_model_state_make_high.h"
#include "dpactor_model_state_run_high.h"

namespace dragonpoop
{

    //ctor
    dpactor_model_state_make_high::dpactor_model_state_make_high( dpactor *a ) : dpactor_model_state( a )
    {

    }

    //dtor
    dpactor_model_state_make_high::~dpactor_model_state_make_high( void )
    {

    }

    //make state
    void dpactor_model_state_make_high::run( dpthread_lock *thd, dpactor *a, dpactor_model_state **next_state )
    {
        this->makeHigh( thd );
        this->killMed();
        this->killLow();
        *next_state = new dpactor_model_state_run_high( a );
    }

};
