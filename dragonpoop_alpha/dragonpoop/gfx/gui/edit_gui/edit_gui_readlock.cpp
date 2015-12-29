
#include "edit_gui_readlock.h"
#include "edit_gui.h"

namespace dragonpoop
{

    //ctor
    edit_gui_readlock::edit_gui_readlock( edit_gui *t, dpmutex_readlock *l ) : gui_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    edit_gui_readlock::~edit_gui_readlock( void )
    {

    }

    //returns true if clicked
    bool edit_gui_readlock::wasClicked( void )
    {
        return this->t->wasClicked();
    }

    //get value
    void edit_gui_readlock::getValue( std::string *s )
    {
        this->t->getValue( s );
    }

};
