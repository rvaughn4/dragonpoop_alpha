
#ifndef dragonpoop_edit_gui_writelock_h
#define dragonpoop_edit_gui_writelock_h

#include "../gui_writelock.h"
#include <string>

namespace dragonpoop
{

    class edit_gui;

    class edit_gui_writelock : public gui_writelock
    {

    private:

        edit_gui *t;

    protected:

        //ctor
        edit_gui_writelock( edit_gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~edit_gui_writelock( void );

    public:

        //returns true if clicked
        bool wasClicked( void );
        //set value
        void setValue( std::string *s );
        //get value
        void getValue( std::string *s );

        friend class edit_gui;
    };

};

#endif
