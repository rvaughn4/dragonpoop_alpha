
#include "menu_gui_readlock.h"
#include "menu_gui.h"

namespace dragonpoop
{

    //ctor
    menu_gui_readlock::menu_gui_readlock( menu_gui *t, dpmutex_readlock *l ) : gui_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    menu_gui_readlock::~menu_gui_readlock( void )
    {

    }

    //returns true if button was clicked
    bool menu_gui_readlock::wasClicked( const char *btn_name )
    {
        return this->t->wasClicked( btn_name );
    }

    //returns true if closed
    bool menu_gui_readlock::wasClosed( void )
    {
        return this->t->wasClosed();
    }

};
