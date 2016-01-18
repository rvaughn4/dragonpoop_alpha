
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
        float w, h, fx, fy;

        l = (input_passer_writelock *)o.tryWriteLock( this->ip, 100, "window::addMouseInput" );
        if( !l )
            return;

        w = this->getWidth();
        h = this->getHeight();
        fx = 0;
        fy = 0;

        if( w > 0 )
            fx = (float)x / w;
        if( h > 0 )
            fy = (float)y / h;
        fx = ( 2.0f * fx ) - 1.0f;
        fy = ( 2.0f * fy ) - 1.0f;

        l->addMouseInput( fx, -fy, bLb, bRb, bDown );
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
