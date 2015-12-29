
#include "edit_text_gui_readlock.h"
#include "edit_text_gui.h"

namespace dragonpoop
{

    //ctor
    edit_text_gui_readlock::edit_text_gui_readlock( edit_text_gui *t, dpmutex_readlock *l ) : gui_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    edit_text_gui_readlock::~edit_text_gui_readlock( void )
    {

    }

    //returns true if clicked
    bool edit_text_gui_readlock::wasClicked( void )
    {
        return this->t->wasClicked();
    }

};
