
#include "input_passer_writelock.h"
#include "input_passer.h"

namespace dragonpoop
{

    //ctor
    input_passer_writelock::input_passer_writelock( input_passer *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    input_passer_writelock::~input_passer_writelock( void )
    {

    }

    //get position time
    uint64_t input_passer_writelock::getTime( void )
    {
        return this->t->getTime();
    }

    //returns true if has input
    bool input_passer_writelock::hasInput( void )
    {
        return this->t->hasInput();
    }

    //returns true if has mouse input
    bool input_passer_writelock::hasMouseInput( void )
    {
        return this->t->hasMouseInput();
    }

    //returns true if has keyboard input
    bool input_passer_writelock::hasKeyboardInput( void )
    {
        return this->t->hasKeyboardInput();
    }

    //set time
    void input_passer_writelock::setTime( uint64_t t )
    {
        this->t->setTime( t );
    }

    //sync
    void input_passer_writelock::sync( input_passer_ref *r )
    {
        this->t->sync( r );
    }

    //retrieve keyboard input
    bool input_passer_writelock::getKeyboardInput( std::string *sname, bool *bIsDown )
    {
        return this->t->getKeyboardInput( sname, bIsDown );
    }

    //retrieve mouse input
    bool input_passer_writelock::getMouseInput( float *px, float *py, bool *bIsLeft, bool *bIsRight, bool *bIsDown )
    {
        return this->t->getMouseInput( px, py, bIsLeft, bIsRight, bIsDown );
    }

    //add keyboard input
    bool input_passer_writelock::addKeyboardInput( std::string *sname, bool bIsDown )
    {
        return this->t->addKeyboardInput( sname, bIsDown );
    }

    //add mouse input
    bool input_passer_writelock::addMouseInput( float x, float y, bool bIsLeft, bool bIsRight, bool bIsDown )
    {
        return this->t->addMouseInput( x, y, bIsLeft, bIsRight, bIsDown );
    }

    //clear all input
    void input_passer_writelock::clear( void )
    {
        this->t->clear();
    }

    //set matrix
    void input_passer_writelock::setMatrix( dpmatrix *m )
    {
        this->t->setMatrix( m );
    }

    //get matrix
    void input_passer_writelock::getMatrix( dpmatrix *m )
    {
        this->t->getMatrix( m );
    }

};
