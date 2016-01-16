
#ifndef dragonpoop_window_gui_h
#define dragonpoop_window_gui_h

#include "../gui.h"

namespace dragonpoop
{

    class button_gui;

    class window_gui : public gui
    {

    private:

        button_gui *bclose, *bhide, *btitle;
        bool bDoClose;

    protected:

        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        //override to do processing
        virtual void doProcessing( dpthread_lock *thd, gui_writelock *g );
        //close this window
        void close( void );

    public:

        //ctor
        window_gui( gfx_writelock *g, dpid id, int x, int y, int w, int h, const char *ctitle );
        //dtor
        virtual ~window_gui( void );
        //returns true if closed
        bool wasClosed( void );

    };

};

#endif
