
#include "button_gui.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

#include <iostream>

namespace dragonpoop
{

    //ctor
    button_gui::button_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txt, bool bHasBg ) : gui( g, id )
    {
        std::string s( txt );

        this->setParentId( pid );
        this->enableBg( bHasBg );
        this->enableFg( 1 );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setText( &s );
        this->setHoverMode( bHasBg );
        this->setEditMode( 0 );
        this->setFade( 0 );
        this->bWasClicked = 0;

        x = h * 8 / 10;
        y = (float)s.length();
        if( y > 0 )
        {
            y = w / y * 1.8f;
            if( y < x )
                x = y;
        }
        y = ( h - x ) / 3;

        this->setFontSize( x );
        this->setMargin( y );
    }

    //dtor
    button_gui::~button_gui( void )
    {
        shared_obj_guard o;
        o.tryWriteLock( this, 5000, "button_gui::~button_gui" );
        o.unlock();
        this->unlink();
    }

    //override to paint background texture
    void button_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }

    //returns true if clicked
    bool button_gui::wasClicked( void )
    {
        bool r = this->bWasClicked;
        this->bWasClicked = 0;
        return r;
    }

    //override to handle mouse button
    bool button_gui::handleMouseClick( float x, float y, bool isRight, bool isDown )
    {
        if( !this->gui::handleMouseClick( x, y, isRight, isDown ) )
            return 0;
        this->bWasClicked = this->bWasClicked | isDown;
        return 1;
    }

};
