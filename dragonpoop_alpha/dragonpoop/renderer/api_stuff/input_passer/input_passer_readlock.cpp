
#include "input_passer_readlock.h"
#include "input_passer.h"

namespace dragonpoop
{

    //ctor
    input_passer_readlock::input_passer_readlock( input_passer *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    input_passer_readlock::~input_passer_readlock( void )
    {

    }

    //get position time
    uint64_t input_passer_readlock::getTime( void )
    {
        return this->t->getTime();
    }

    //returns true if has input
    bool input_passer_readlock::hasInput( void )
    {
        return this->t->hasInput();
    }

    //returns true if has mouse input
    bool input_passer_readlock::hasMouseInput( void )
    {
        return this->t->hasMouseInput();
    }

    //returns true if has keyboard input
    bool input_passer_readlock::hasKeyboardInput( void )
    {
        return this->t->hasKeyboardInput();
    }

    //get matrix
    void input_passer_readlock::getMatrix( dpmatrix *m )
    {
        this->t->getMatrix( m );
    }

};
