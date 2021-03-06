
#ifndef dragonpoop_button_gui_h
#define dragonpoop_button_gui_h

#include "../gui.h"

namespace dragonpoop
{

    class button_gui : public gui
    {

    private:

        bool bWasClicked;

    protected:

        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //override to handle mouse button
        virtual bool handleMouseClick( float x, float y, float sx, float sy, bool isRight, bool isDown );

    public:

        //ctor
        button_gui( gfx_writelock *g, dpid id, float x, float y, float w, float h, const char *txt, bool bHasBg );
        //dtor
        virtual ~button_gui( void );
        //returns true if clicked
        bool wasClicked( void );

    };

};

#endif
