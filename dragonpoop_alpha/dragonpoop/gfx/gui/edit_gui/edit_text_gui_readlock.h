
#ifndef dragonpoop_edit_text_gui_readlock_h
#define dragonpoop_edit_text_gui_readlock_h

#include "../gui_readlock.h"
#include <string>

namespace dragonpoop
{

    class edit_text_gui;

    class edit_text_gui_readlock : public gui_readlock
    {

    private:

        edit_text_gui *t;

    protected:

        //ctor
        edit_text_gui_readlock( edit_text_gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~edit_text_gui_readlock( void );

    public:

        //returns true if clicked
        bool wasClicked( void );

        friend class edit_text_gui;
    };

};

#endif
