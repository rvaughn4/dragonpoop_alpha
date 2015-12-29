
#ifndef dragonpoop_edit_gui_readlock_h
#define dragonpoop_edit_gui_readlock_h

#include "../gui_readlock.h"
#include <string>

namespace dragonpoop
{

    class edit_gui;

    class edit_gui_readlock : public gui_readlock
    {

    private:

        edit_gui *t;

    protected:

        //ctor
        edit_gui_readlock( edit_gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~edit_gui_readlock( void );

    public:

        //returns true if clicked
        bool wasClicked( void );
        //get value
        void getValue( std::string *s );

        friend class edit_gui;
    };

};

#endif
