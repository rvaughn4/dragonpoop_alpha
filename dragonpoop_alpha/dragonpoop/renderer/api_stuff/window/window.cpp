
#include "window.h"
#include "../input_passer/input_passer_ref.h"
#include "../input_passer/input_passer_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    window::window( float w, float h, const char *ctitle, input_passer_writelock *ipl )
    {
        this->ip = 0;
        if( ipl )
            this->ip = (input_passer_ref *)ipl->getRef();
    }

    //dtor
    window::~window( void )
    {
        delete this->ip;
    }

    //run window
    void window::run( void )
    {

    }

    //returns true if window is open
    bool window::isOpen( void )
    {
        return 0;
    }

    //returns window width
    float window::getWidth( void )
    {
        return 0;
    }

    //returns window height
    float window::getHeight( void )
    {
        return 0;
    }

    //adds mouse input
    void window::addMouseInput( uint64_t t, int x, int y, bool bLb, bool bRb, bool bDown )
    {
        shared_obj_guard o;
        input_passer_writelock *l;

        l = (input_passer_writelock *)o.tryWriteLock( this->ip, 100, "window::addMouseInput" );
        if( !l )
            return;

        l->addMouseInput( x, y, bLb, bRb, bDown );
        l->setTime( t );
    }

    //adds kb input
    void window::addKBInput( uint64_t t, std::string *skey, bool bDown )
    {
        shared_obj_guard o;
        input_passer_writelock *l;

        l = (input_passer_writelock *)o.tryWriteLock( this->ip, 100, "window::addMouseInput" );
        if( !l )
            return;

        l->addKeyboardInput( skey, bDown );
        l->setTime( t );
    }

};
