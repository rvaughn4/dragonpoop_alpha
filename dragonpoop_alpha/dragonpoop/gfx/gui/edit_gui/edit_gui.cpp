
#include "edit_gui.h"
#include "edit_gui_readlock.h"
#include "edit_gui_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "edit_text_gui.h"
#include "edit_text_gui_readlock.h"
#include "edit_text_gui_writelock.h"

namespace dragonpoop
{

    //ctor
    edit_gui::edit_gui( gfx_writelock *g, dpid id, float x, float y, float w, float h, const char *txtLabel, const char *txtValue, float labelWidth, bool bHasBg ) : gui( g, id )
    {
        std::string s( txtLabel );

        this->enableBg( bHasBg );
        this->enableFg( 1 );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setText( &s );
        this->setHoverMode( 0 );
        this->setEditMode( 0 );
        this->setFade( 0 );

        this->txt_gui = new edit_text_gui( g, this->genId(), labelWidth, 0, w - labelWidth, h, txtValue );
        this->addGui( this->txt_gui );

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
    edit_gui::~edit_gui( void )
    {
        shared_obj_guard o;
        o.tryWriteLock( this, 5000, "edit_gui::~edit_gui" );
        o.unlock();
        this->unlink();
        delete this->txt_gui;
    }

    //generate read lock
    shared_obj_readlock *edit_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new edit_gui_readlock( (edit_gui *)p, l );
    }

    //generate write lock
    shared_obj_writelock *edit_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new edit_gui_writelock( (edit_gui *)p, l );
    }

    //override to paint background texture
    void edit_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_button.bmp" );
    }

    //returns true if clicked
    bool edit_gui::wasClicked( void )
    {
        shared_obj_guard o;
        edit_text_gui_readlock *l;

        l = (edit_text_gui_readlock *)o.tryReadLock( this->txt_gui, 100, "edit_gui::wasClicked" );
        if( !l )
            return 0;

        return l->wasClicked();
    }

    //set value
    void edit_gui::setValue( std::string *s )
    {
        shared_obj_guard o;
        edit_text_gui_writelock *l;

        l = (edit_text_gui_writelock *)o.tryWriteLock( this->txt_gui, 1000, "edit_gui::setValue" );
        if( !l )
            return;

        l->setText( s );
    }

    //get value
    void edit_gui::getValue( std::string *s )
    {
        shared_obj_guard o;
        edit_text_gui_readlock *l;

        l = (edit_text_gui_readlock *)o.tryReadLock( this->txt_gui, 1000, "edit_gui::getValue" );
        if( !l )
            return;

        l->getText( s );
    }

};
