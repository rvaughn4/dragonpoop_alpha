
#ifndef dragonpoop_edit_text_gui_writelock_h
#define dragonpoop_edit_text_gui_writelock_h

#include "../gui_writelock.h"
#include <string>

namespace dragonpoop
{

    class edit_text_gui;

    class edit_text_gui_writelock : public gui_writelock
    {

    private:

        edit_text_gui *t;

    protected:

        //ctor
        edit_text_gui_writelock( edit_text_gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~edit_text_gui_writelock( void );

    public:

        //returns true if clicked
        bool wasClicked( void );

        friend class edit_text_gui;
    };

};

#endif
