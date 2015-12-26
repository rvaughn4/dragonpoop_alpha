
#ifndef dragonpoop_edit_gui_h
#define dragonpoop_edit_gui_h

#include "../gui.h"

namespace dragonpoop
{

    class edit_gui : public gui
    {

    private:

        bool bWasClicked;

    protected:

        //returns true if clicked
        bool wasClicked( void );
        //set value

        //get value

    public:

        //ctor
        edit_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txtLabel, const char *txtValue, float labelWidth );
        //dtor
        virtual ~edit_gui( void );

    };

};

#endif
