
#ifndef dragonpoop_x11_window_h
#define dragonpoop_x11_window_h

#include "../window/window.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#include <dlfcn.h>

namespace dragonpoop
{

//    typedef int opengl1o5_x11_GLint;
    typedef Display x11_window_Display;
    typedef XVisualInfo x11_window_XVisualInfo;
    typedef Colormap x11_window_Colormap;
    typedef Visual x11_window_Visual;
    typedef Window x11_window_Window;
    typedef XSetWindowAttributes x11_window_XSetWindowAttributes;
    typedef Atom x11_window_Atom;
    typedef Pixmap x11_window_Pixmap;
    typedef XSizeHints x11_window_XSizeHints;
    typedef XEvent x11_window_XEvent;
    typedef XSelectionEvent x11_window_XSelectionEvent;
    typedef KeySym x11_window_KeySym;
    typedef XKeyEvent x11_window_XKeyEvent;
    typedef Time x11_window_Time;

    #define x11_window_GLX_RGBA 4
    #define x11_window_GLX_RED_SIZE		8
    #define x11_window_GLX_GREEN_SIZE		9
    #define x11_window_GLX_BLUE_SIZE		10
    #define x11_window_GLX_ALPHA_SIZE		11
    #define x11_window_None 0
    #define x11_window_GLX_DEPTH_SIZE		12
    #define x11_window_GLX_DOUBLEBUFFER	5
    #define x11_window_AllocNone		0
    #define x11_window_PropertyChangeMask		(1L<<22)
    #define x11_window_KeyPressMask			(1L<<0)
    #define x11_window_KeyReleaseMask			(1L<<1)
    #define x11_window_ButtonPressMask			(1L<<2)
    #define x11_window_ButtonReleaseMask		(1L<<3)
    #define x11_window_StructureNotifyMask		(1L<<17)
    #define x11_window_PointerMotionMask		(1L<<6)
    #define x11_window_InputOutput		1
    #define x11_window_CWBorderPixel           (1L<<3)
    #define x11_window_CWEventMask		(1L<<11)
    #define x11_window_CWColormap		(1L<<13)
    #define x11_window_Button1			1
    #define x11_window_Button2			2
    #define x11_window_SelectionNotify		31
    #define x11_window_PropModeReplace         0
    #define x11_window_PropertyChangeMask		(1L<<22)
    #define x11_window_CurrentTime          0L	/* special Time */


    typedef void (* x11_window_functions_XInitThreads )( void );
    typedef x11_window_Display *(* x11_window_functions_XOpenDisplay )( char *display_name );
    typedef int (* x11_window_functions_XDefaultScreen )( x11_window_Display *dpy );
    typedef x11_window_XVisualInfo *(* x11_window_functions_glXChooseVisual )( x11_window_Display *dpy, int screen, int *attr_list );
    typedef void (* x11_window_functions_XCloseDisplay )( x11_window_Display *dpy );
    typedef x11_window_Colormap (* x11_window_functions_XCreateColormap )( x11_window_Display *dpy, x11_window_Window w, x11_window_Visual *vis, int alloc );
    typedef x11_window_Window (* x11_window_functions_XRootWindow )( x11_window_Display *dpy, int screen );
    typedef x11_window_Window (* x11_window_functions_XCreateWindow )( x11_window_Display *dpy, x11_window_Window parent, int x, int y, unsigned int w, unsigned int h, unsigned int border_width, int depth, unsigned int _class, x11_window_Visual *vi, unsigned long value_mask, x11_window_XSetWindowAttributes *attrs );
    typedef x11_window_Atom (* x11_window_functions_XInternAtom )( x11_window_Display *dpy, char *name, bool only_if_exists );
    typedef int (* x11_window_functions_XSetWMProtocols )( x11_window_Display *dpy, x11_window_Window w, x11_window_Atom *protocols, int count );
    typedef void (* x11_window_functions_XSetStandardProperties )( x11_window_Display *dpy, x11_window_Window w, char *window_name, char *icon_name, x11_window_Pixmap ico, char **argv, int argc, x11_window_XSizeHints *hints );
    typedef void (* x11_window_functions_XMapRaised )( x11_window_Display *dpy, x11_window_Window w );
    typedef void (* x11_window_functions_XDestroyWindow )( x11_window_Display *dpy, x11_window_Window w );
    typedef int (* x11_window_functions_XPending )( x11_window_Display *dpy );
    typedef void (* x11_window_functions_XNextEvent )( x11_window_Display *dpy, x11_window_XEvent *event_return );
    typedef x11_window_KeySym (* x11_window_functions_XLookupKeysym )( x11_window_XKeyEvent *key_event, int index );
    typedef void (* x11_window_functions_XChangeProperty )( x11_window_Display *dpy, x11_window_Window w, x11_window_Atom property, x11_window_Atom type, int format, int mode, unsigned char *data, int nelements );
    typedef int (* x11_window_functions_XSendEvent )( x11_window_Display *dpy, x11_window_Window w, bool propagate, long event_mask, x11_window_XEvent *event_send );
    typedef void (* x11_window_functions_XSetSelectionOwner )( x11_window_Display *dpy, x11_window_Atom selection, x11_window_Window w, x11_window_Time _time );
    typedef void (* x11_window_functions_XConvertSelection )( x11_window_Display *dpy, x11_window_Atom selection, x11_window_Atom target, x11_window_Atom property, x11_window_Window w, x11_window_Time _time );
    typedef int (* x11_window_functions_XGetWindowProperty )( x11_window_Display *dpy, x11_window_Window w, x11_window_Atom property, long long_offset, long long_length, bool _delete, x11_window_Atom req_type, x11_window_Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return );
    typedef int (* x11_window_functions_XFree )( void *data );

    struct x11_window_functions
    {
        x11_window_functions_XInitThreads XInitThreads;
        x11_window_functions_XOpenDisplay XOpenDisplay;
        x11_window_functions_XDefaultScreen XDefaultScreen;
        x11_window_functions_glXChooseVisual glXChooseVisual;
        x11_window_functions_XCloseDisplay XCloseDisplay;
        x11_window_functions_XCreateColormap XCreateColormap;
        x11_window_functions_XRootWindow XRootWindow;
        x11_window_functions_XCreateWindow XCreateWindow;
        x11_window_functions_XInternAtom XInternAtom;
        x11_window_functions_XSetWMProtocols XSetWMProtocols;
        x11_window_functions_XSetStandardProperties XSetStandardProperties;
        x11_window_functions_XMapRaised XMapRaised;
        x11_window_functions_XDestroyWindow XDestroyWindow;
        x11_window_functions_XPending XPending;
        x11_window_functions_XNextEvent XNextEvent;
        x11_window_functions_XLookupKeysym XLookupKeysym;
        x11_window_functions_XChangeProperty XChangeProperty;
        x11_window_functions_XSendEvent XSendEvent;
        x11_window_functions_XSetSelectionOwner XSetSelectionOwner;
        x11_window_functions_XConvertSelection XConvertSelection;
        x11_window_functions_XGetWindowProperty XGetWindowProperty;
        x11_window_functions_XFree XFree;
    };

    class x11_window : public window
    {

    private:

        x11_window_Display *dpy;
        int screen;
        x11_window_Window win;
        x11_window_XSetWindowAttributes attr;
        x11_window_Atom wm_delete_window, selprop, a_targets, a_clipboard, a_string, a_primary;
        int w, h;
        bool lb, rb, bIsOpen, bctrl, bdocut, bshift, bcaps;
        x11_window_XVisualInfo *vi;

        x11_window_functions x11;
        void *dlX11, *dlGL;

        //load x11
        bool loadFunctions( void );
        //process kb input
        void processKb( x11_window_KeySym k, bool isDown );

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
        x11_window_Window getWindow( void );
        //return screen
        int getScreen( void );
        //return display
        x11_window_Display *getDisplay( void );
        //return vi
        x11_window_XVisualInfo *getVi( void );

    };

};

#endif
