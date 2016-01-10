
#include "input_passer.h"
#include "input_passer_ref.h"
#include "input_passer_readlock.h"
#include "input_passer_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    input_passer::input_passer( dpmutex_master *mm ) : shared_obj( mm )
    {
        std::shared_ptr<std::atomic<uint64_t>> tt( new std::atomic<uint64_t>() );
        this->t = tt;
        this->clear();
    }

    //dtor
    input_passer::~input_passer( void )
    {
    }

    //generate write lock
    shared_obj_writelock *input_passer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new input_passer_writelock( (input_passer *)p, l );
    }

    //generate read lock
    shared_obj_readlock *input_passer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new input_passer_readlock( (input_passer *)p, l );
    }

    //generate ref
    shared_obj_ref *input_passer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new input_passer_ref( (input_passer *)p, k, this->t );
    }

    //get position time
    uint64_t input_passer::getTime( void )
    {
        return *( this->t.get() );
    }

    //set time
    void input_passer::setTime( uint64_t t )
    {
        *( this->t.get() ) = t;
    }

    //clear
    void input_passer::clear( void )
    {
        memset( &this->input, 0, sizeof( this->input ) );
    }

    //sync
    void input_passer::sync( input_passer_ref *r )
    {
        input_passer_writelock *l;
        shared_obj_guard o;
        unsigned int iw, ih, is, ii, ie;

        if( r->getTime() == this->getTime() )
            return;

        l = (input_passer_writelock *)o.tryWriteLock( r, 30, "input_passer::sync" );
        if( !l )
            return;

        *( this->t.get() ) = l->getTime();

    }

    //returns true if has input
    bool input_passer::hasInput( void )
    {
        return this->hasMouseInput() || this->hasKeyboardInput();
    }

    //returns true if has mouse input
    bool input_passer::hasMouseInput( void )
    {
        return this->input.mouse.cnt > 0;
    }

    //returns true if has keyboard input
    bool input_passer::hasKeyboardInput( void )
    {
        return this->input.keyb.cnt > 0;
    }

    //retrieve keyboard input
    bool input_passer::getKeyboardInput( std::string *sname, bool *bIsDown )
    {
        return this->_popKB( sname, bIsDown );
    }

    //retrieve mouse input
    bool input_passer::getMouseInput( int *px, int *py, bool *bIsLeft, bool *bIsRight, bool *bIsDown )
    {
        return this->_popMouse( px, py, bIsLeft, bIsRight, bIsDown );
    }

    //add keyboard input
    bool input_passer::addKeyboardInput( std::string *sname, bool bIsDown )
    {
        return this->_pushKB( sname, bIsDown );
    }

    //add mouse input
    bool input_passer::addMouseInput( int x, int y, bool bIsLeft, bool bIsRight, bool bIsDown )
    {
        return this->_pushMouse( x, y, bIsLeft, bIsRight, bIsDown );
    }

};
