
#include "button_gui.h"

namespace dragonpoop
{
    
    //ctor
    button_gui::button_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txt ) : gui( g, id )
    {
        this->setParentId( pid );
        this->enableBg( 1 );
        this->enableFg( 1 );
        this->setFontSize( h * 9 / 10 );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setText( txt );
        this->setHoverMode( 1 );
        this->setEditMode( 1 );
    }
    
    //dtor
    button_gui::~button_gui( void )
    {
        
    }
    
    //override to paint background texture
    void button_gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        bm->loadFile( "white_gui_box.bmp" );
    }
    
};
