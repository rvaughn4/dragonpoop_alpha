
#include "edit_text_gui_writelock.h"
#include "edit_text_gui.h"

namespace dragonpoop
{

    //ctor
    edit_text_gui_writelock::edit_text_gui_writelock( edit_text_gui *t, dpmutex_writelock *l ) : gui_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    edit_text_gui_writelock::~edit_text_gui_writelock( void )
    {

    }

    //returns true if clicked
    bool edit_text_gui_writelock::wasClicked( void )
    {
        return this->t->wasClicked();
    }

};
