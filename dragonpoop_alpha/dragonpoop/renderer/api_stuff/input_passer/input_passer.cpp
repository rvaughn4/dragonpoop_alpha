
#include "input_passer.h"
#include "input_passer_ref.h"
#include "input_passer_readlock.h"
#include "input_passer_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include <string.h>

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
        std::string skey;
        bool bDown, bLb, bRb;
        int x, y;

        if( r->getTime() == this->getTime() )
            return;

        l = (input_passer_writelock *)o.tryWriteLock( r, 30, "input_passer::sync" );
        if( !l )
            return;

        *( this->t.get() ) = l->getTime();

        while( l->getMouseInput( &x, &y, &bLb, &bRb, &bDown ) )
            this->addMouseInput( x, y, bLb, bRb, bDown );
        while( l->getKeyboardInput( &skey, &bDown ) )
            this->addKeyboardInput( &skey, bDown );
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

    //attempts to push keyboard input at current index, increments index
    bool input_passer::_pushKBOnce( std::string *sname, bool bDown )
    {
        input_passer_kb_input *v;

        if( this->input.keyb.push_index >= input_passer_MAX_INPUT )
            this->input.keyb.push_index = 0;

        v = &this->input.keyb.values[ this->input.keyb.push_index ];
        if( v->bAlive )
            return 0;

        v->bAlive = 1;
        v->bDown = bDown;
        memset( v->key, 0, sizeof( v->key ) );
        sname->copy( v->key, sizeof( v->key ) );

        this->input.keyb.cnt++;
        return 1;
    }

    //attempts to push mouse input at current index, increments index
    bool input_passer::_pushMouseOnce( int x, int y, bool bLb, bool bRb, bool bDown )
    {
        input_passer_mouse_input *v;

        if( this->input.mouse.push_index >= input_passer_MAX_INPUT )
            this->input.mouse.push_index = 0;

        v = &this->input.mouse.values[ this->input.mouse.push_index ];
        if( v->bAlive )
            return 0;

        v->bAlive = 1;
        v->bDown = bDown;
        v->isLb = bLb;
        v->isRb = bRb;
        v->x = x;
        v->y = y;

        this->input.mouse.cnt++;
        return 1;
    }

    //calls pushkbonce until unused array index found or max tries
    bool input_passer::_pushKB( std::string *sname, bool bDown )
    {
        unsigned int i;

        for( i = 0; i < input_passer_MAX_INPUT; i++ )
        {
            if( this->_pushKBOnce( sname, bDown ) )
                return 1;
        }

        return 0;
    }

    //calls pushmouseonce until unused array index found or max tries
    bool input_passer::_pushMouse( int x, int y, bool bLb, bool bRb, bool bDown )
    {
        unsigned int i;

        for( i = 0; i < input_passer_MAX_INPUT; i++ )
        {
            if( this->_pushMouseOnce( x, y, bLb, bRb, bDown ) )
                return 1;
        }

        return 0;
    }

    //attempts to pop keyboard input at current index, increments index
    bool input_passer::_popKBOnce( std::string *sname, bool *bDown )
    {
        input_passer_kb_input *v;

        if( this->input.keyb.pop_index >= input_passer_MAX_INPUT )
            this->input.keyb.pop_index = 0;

        v = &this->input.keyb.values[ this->input.keyb.pop_index ];
        if( !v->bAlive )
            return 0;

        v->bAlive = 0;
        *bDown = v->bDown;
        v->key[ sizeof( v->key ) - 1 ] = 0;
        sname->assign( (const char *)v->key );

        this->input.keyb.cnt--;
        return 1;
    }

    //attempts to pop mouse input at current index, increments index
    bool input_passer::_popMouseOnce( int *x, int *y, bool *bLb, bool *bRb, bool *bDown )
    {
        input_passer_mouse_input *v;

        if( this->input.mouse.pop_index >= input_passer_MAX_INPUT )
            this->input.mouse.pop_index = 0;

        v = &this->input.mouse.values[ this->input.mouse.pop_index ];
        if( !v->bAlive )
            return 0;

        v->bAlive = 0;
        *bDown = v->bDown;
        *x = v->x;
        *y = v->y;
        *bLb = v->isLb;
        *bRb = v->isRb;

        this->input.mouse.cnt--;
        return 1;
    }

    //calls popkbonce until used array index found or max tries
    bool input_passer::_popKB( std::string *sname, bool *bDown )
    {
        unsigned int i;

        for( i = 0; i < input_passer_MAX_INPUT; i++ )
        {
            if( this->_popKBOnce( sname, bDown ) )
                return 1;
        }

        return 0;
    }

    //calls popmouseonce until used array index found or max tries
    bool input_passer::_popMouse( int *x, int *y, bool *bLb, bool *bRb, bool *bDown )
    {
        unsigned int i;

        for( i = 0; i < input_passer_MAX_INPUT; i++ )
        {
            if( this->_popMouseOnce( x, y, bLb, bRb, bDown ) )
                return 1;
        }

        return 0;
    }

};
