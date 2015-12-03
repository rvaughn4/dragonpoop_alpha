
#ifndef dragonpoop_menu_gui_readlock_h
#define dragonpoop_menu_gui_readlock_h

#include "../gui_readlock.h"

namespace dragonpoop
{
    class menu_gui;

    class menu_gui_readlock : public gui_readlock
    {

    private:

        menu_gui *t;

    protected:

        //ctor
        menu_gui_readlock( menu_gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~menu_gui_readlock( void );

    public:

        //returns true if button was clicked
        bool wasClicked( const char *btn_name );
        //returns true if closed
        bool wasClosed( void );

        friend class menu_gui;
    };

};

#endif
