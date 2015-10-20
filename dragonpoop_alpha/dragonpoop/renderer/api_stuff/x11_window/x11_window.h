
#ifndef dragonpoop_x11_window_h
#define dragonpoop_x11_window_h

#include "../window/window.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

namespace dragonpoop
{
    
    class x11_window : public window
    {
        
    private:
        
        Display *dpy;
        int screen;
        Window win;
        XSetWindowAttributes attr;
        Atom wm_delete_window, selprop, a_targets, a_clipboard;
        int w, h;
        bool lb, rb, bIsOpen;
        XVisualInfo *vi;

    protected:
        
    public:
        
        //ctor
        x11_window( float w, float h, const char *ctitle );
        //dtor
        virtual ~x11_window( void );
        //run window
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        //returns window width
        virtual float getWidth( void );
        //returns window height
        virtual float getHeight( void );
        //return x11 window
        Window getWindow( void );
        //return screen
        int getScreen( void );
        //return display
        Display *getDisplay( void );
        //return vi
        XVisualInfo *getVi( void );
        
    };
    
};

#endif