
#include "menu_gui_writelock.h"
#include "menu_gui.h"

namespace dragonpoop
{

    //ctor
    menu_gui_writelock::menu_gui_writelock( menu_gui *t, dpmutex_writelock *l ) : gui_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    menu_gui_writelock::~menu_gui_writelock( void )
    {

    }

    //add button
    void menu_gui_writelock::addButton( const char *btn_name )
    {
        this->t->addButton( btn_name );
    }

    //remove button
    void menu_gui_writelock::removeButton( const char *btn_name )
    {
        this->t->removeButton( btn_name );
    }

    //remove all buttons
    void menu_gui_writelock::removeButtons( void )
    {
        this->t->removeButtons();
    }

    //returns true if button was clicked
    bool menu_gui_writelock::wasClicked( const char *btn_name )
    {
        return this->t->wasClicked( btn_name );
    }

    //returns true if closed
    bool menu_gui_writelock::wasClosed( void )
    {
        return this->t->wasClosed();
    }

    //returns true if clicked, retreives button name
    bool menu_gui_writelock::getClicked( std::string *sname )
    {
        return this->t->getClicked( sname );
    }

};
