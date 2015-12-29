
#include "edit_gui_writelock.h"
#include "edit_gui.h"

namespace dragonpoop
{

    //ctor
    edit_gui_writelock::edit_gui_writelock( edit_gui *t, dpmutex_writelock *l ) : gui_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    edit_gui_writelock::~edit_gui_writelock( void )
    {

    }

    //returns true if clicked
    bool edit_gui_writelock::wasClicked( void )
    {
        return this->t->wasClicked();
    }

    //set value
    void edit_gui_writelock::setValue( std::string *s )
    {
        this->t->setValue( s );
    }

    //get value
    void edit_gui_writelock::getValue( std::string *s )
    {
        this->t->getValue( s );
    }

};
