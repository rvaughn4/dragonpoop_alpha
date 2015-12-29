
#include "edit_text_gui.h"
#include "edit_text_gui_readlock.h"
#include "edit_text_gui_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    edit_text_gui::edit_text_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txtValue ) : gui( g, id )
    {
        std::string s( txtValue );

        this->setParentId( pid );
        this->enableBg( 1 );
        this->enableFg( 1 );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setText( &s );
        this->setHoverMode( 1 );
        this->setEditMode( 1 );
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
    edit_text_gui::~edit_text_gui( void )
    {
        shared_obj_guard o;
        o.tryWriteLock( this, 5000, "edit_text_gui::~edit_text_gui" );
        o.unlock();
        this->unlink();
    }

    //generate read lock
    shared_obj_readlock *edit_text_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new edit_text_gui_readlock( (edit_text_gui *)p, l );
    }

    //generate write lock
    shared_obj_writelock *edit_text_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new edit_text_gui_writelock( (edit_text_gui *)p, l );
    }

    //override to paint background texture
    void edit_text_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_button.bmp" );
    }

    //returns true if clicked
    bool edit_text_gui::wasClicked( void )
    {
        return this->bWasClicked;
    }

    //override to handle keyboard button
    bool edit_text_gui::handleKbEvent( std::string *skey, bool isDown )
    {
        if( ( skey->compare( "return" ) == 0 || skey->compare( "enter" ) == 0 ) && isDown )
            this->bWasClicked = 1;

        return this->gui::handleKbEvent( skey, isDown );
    }

};
