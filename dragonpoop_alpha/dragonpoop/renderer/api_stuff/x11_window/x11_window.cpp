
#include "x11_window.h"
#include "../window/window.h"
#include <string>
#include <string.h>

namespace dragonpoop
{

    //ctor
    x11_window::x11_window( float w, float h, const char *ctitle ) : window( w, h, ctitle )
    {
        x11_window_Colormap cmap;

        this->bIsOpen = 0;
        this->dpy = 0;
        this->win = 0;
        this->screen = 0;
        this->dlX11 = 0;
        this->dlGL = 0;
        this->lb = this->rb = 0;
        this->bcaps = 0;
        this->bctrl = 0;
        this->bdocut = 0;
        this->bshift = 0;

        if( !this->loadFunctions() )
            return;

        //open display
        this->x11.XInitThreads();
        this->dpy = this->x11.XOpenDisplay( 0 );
        this->screen = this->x11.XDefaultScreen( this->dpy );

        //set visual format
        static int attrListSgl[] =
        {
            x11_window_GLX_RGBA,
            x11_window_GLX_RED_SIZE, 4,
            x11_window_GLX_GREEN_SIZE, 4,
            x11_window_GLX_BLUE_SIZE, 4,
            x11_window_GLX_DEPTH_SIZE, 16,
            x11_window_None
        };
        static int attrListDbl[] =
        {
            x11_window_GLX_RGBA, x11_window_GLX_DOUBLEBUFFER,
            x11_window_GLX_RED_SIZE, 4,
            x11_window_GLX_GREEN_SIZE, 4,
            x11_window_GLX_BLUE_SIZE, 4,
            x11_window_GLX_DEPTH_SIZE, 16,
            x11_window_None
        };
        vi = this->x11.glXChooseVisual( this->dpy, this->screen, attrListDbl );
        if( !vi )
            vi = this->x11.glXChooseVisual( this->dpy, this->screen, attrListSgl );
        if( !vi )
        {
            this->x11.XCloseDisplay( this->dpy );
            this->dpy = 0;
            return;
        }

        //colormap
        cmap = this->x11.XCreateColormap( this->dpy, this->x11.XRootWindow( this->dpy, vi->screen ), vi->visual, x11_window_AllocNone );
        this->attr.colormap = cmap;
        this->attr.border_pixel = 0;

        //create window
        this->attr.event_mask = x11_window_PropertyChangeMask | x11_window_KeyPressMask | x11_window_KeyReleaseMask | x11_window_ButtonPressMask | x11_window_ButtonReleaseMask | x11_window_StructureNotifyMask | x11_window_PointerMotionMask;
        this->win = this->x11.XCreateWindow( this->dpy, this->x11.XRootWindow( this->dpy, vi->screen ), 0, 0, w, h, 0, vi->depth, x11_window_InputOutput, vi->visual, x11_window_CWBorderPixel | x11_window_CWColormap | x11_window_CWEventMask, &this->attr );
        if( !this->win )
        {
            this->x11.XCloseDisplay( this->dpy );
            this->dpy = 0;
            return;
        }

        //setup close button event
        this->wm_delete_window = this->x11.XInternAtom( this->dpy, (char *)"WM_DELETE_WINDOW", True );
        this->x11.XSetWMProtocols( this->dpy, this->win, &this->wm_delete_window, 1 );

        //selection property
        this->selprop = this->x11.XInternAtom( this->dpy, (char *)"SELECTION_PROPERTY", True );
        this->a_targets = this->x11.XInternAtom( this->dpy, (char *)"TARGETS", 1 );
        this->a_clipboard = this->x11.XInternAtom( this->dpy, (char *)"CLIPBOARD", 1 );
        this->a_string = this->x11.XInternAtom( this->dpy, (char *)"STRING", 1 );
        this->a_primary = this->x11.XInternAtom( this->dpy, (char *)"PRIMARY", 1 );

        //set title
        this->x11.XSetStandardProperties( this->dpy, this->win, (char *)ctitle, (char *)ctitle, x11_window_None, NULL, 0, NULL );
        this->x11.XMapRaised( this->dpy, this->win );

        this->bIsOpen = 1;
    }

    //dtor
    x11_window::~x11_window( void )
    {
        if( this->dpy )
        {
            this->x11.XDestroyWindow( this->dpy, this->win );
            this->x11.XCloseDisplay( this->dpy );
        }
        if( this->dlGL )
            dlclose( this->dlGL );
        if( this->dlX11 )
            dlclose( this->dlX11 );
    }

    //load x11
    bool x11_window::loadFunctions( void )
    {
        bool r;

        memset( &this->x11, 0, sizeof( this->x11 ) );

        this->dlX11 = dlopen( "libX11.so", RTLD_LAZY | RTLD_LOCAL );
        if( !this->dlX11 )
            return 0;
        this->dlGL = dlopen( "libGL.so", RTLD_LAZY | RTLD_LOCAL );
        if( !this->dlGL )
            return 0;

        r = 1;
        r &= ( this->x11.XInitThreads = (x11_window_functions_XInitThreads)dlsym( this->dlX11, "XInitThreads" ) ) != 0;
        r &= ( this->x11.XOpenDisplay = (x11_window_functions_XOpenDisplay)dlsym( this->dlX11, "XOpenDisplay" ) ) != 0;
        r &= ( this->x11.XDefaultScreen = (x11_window_functions_XDefaultScreen)dlsym( this->dlX11, "XDefaultScreen" ) ) != 0;
        r &= ( this->x11.glXChooseVisual = (x11_window_functions_glXChooseVisual)dlsym( this->dlGL, "glXChooseVisual" ) ) != 0;
        r &= ( this->x11.XCloseDisplay = (x11_window_functions_XCloseDisplay)dlsym( this->dlX11, "XCloseDisplay" ) ) != 0;
        r &= ( this->x11.XCreateColormap = (x11_window_functions_XCreateColormap)dlsym( this->dlX11, "XCreateColormap" ) ) != 0;
        r &= ( this->x11.XRootWindow = (x11_window_functions_XRootWindow)dlsym( this->dlX11, "XRootWindow" ) ) != 0;
        r &= ( this->x11.XCreateWindow = (x11_window_functions_XCreateWindow)dlsym( this->dlX11, "XCreateWindow" ) ) != 0;
        r &= ( this->x11.XInternAtom = (x11_window_functions_XInternAtom)dlsym( this->dlX11, "XInternAtom" ) ) != 0;
        r &= ( this->x11.XSetWMProtocols = (x11_window_functions_XSetWMProtocols)dlsym( this->dlX11, "XSetWMProtocols" ) ) != 0;
        r &= ( this->x11.XSetStandardProperties = (x11_window_functions_XSetStandardProperties)dlsym( this->dlX11, "XSetStandardProperties" ) ) != 0;
        r &= ( this->x11.XMapRaised = (x11_window_functions_XMapRaised)dlsym( this->dlX11, "XMapRaised" ) ) != 0;
        r &= ( this->x11.XDestroyWindow = (x11_window_functions_XDestroyWindow)dlsym( this->dlX11, "XDestroyWindow" ) ) != 0;
        r &= ( this->x11.XPending = (x11_window_functions_XPending)dlsym( this->dlX11, "XPending" ) ) != 0;
        r &= ( this->x11.XNextEvent = (x11_window_functions_XNextEvent)dlsym( this->dlX11, "XNextEvent" ) ) != 0;
        r &= ( this->x11.XLookupKeysym = (x11_window_functions_XLookupKeysym)dlsym( this->dlX11, "XLookupKeysym" ) ) != 0;
        r &= ( this->x11.XChangeProperty = (x11_window_functions_XChangeProperty)dlsym( this->dlX11, "XChangeProperty" ) ) != 0;
        r &= ( this->x11.XSendEvent = (x11_window_functions_XSendEvent)dlsym( this->dlX11, "XSendEvent" ) ) != 0;
        r &= ( this->x11.XSetSelectionOwner = (x11_window_functions_XSetSelectionOwner)dlsym( this->dlX11, "XSetSelectionOwner" ) ) != 0;
        r &= ( this->x11.XConvertSelection = (x11_window_functions_XConvertSelection)dlsym( this->dlX11, "XConvertSelection" ) ) != 0;
        r &= ( this->x11.XGetWindowProperty = (x11_window_functions_XGetWindowProperty)dlsym( this->dlX11, "XGetWindowProperty" ) ) != 0;
        r &= ( this->x11.XFree = (x11_window_functions_XFree)dlsym( this->dlX11, "XFree" ) ) != 0;

        return r;
    }


    //run x11_window
    void x11_window::run( void )
    {
        x11_window_XEvent event;
        window_mouse_input m;

        while( this->x11.XPending( this->dpy ) >= 1 )
        {
            this->x11.XNextEvent( this->dpy, &event );
            switch( event.type )
            {
                case ConfigureNotify:
                    if(
                       ( event.xconfigure.width != (int)this->w )
                       ||
                       ( event.xconfigure.height != (int)this->h )
                       )
                    {
                        this->w = event.xconfigure.width;
                        this->h = event.xconfigure.height;
                    }
                    break;
                case MotionNotify:
                    m.x = event.xbutton.x;
                    m.y = event.xbutton.y;
                    m.lb = this->lb;
                    m.rb = this->rb;
                    this->addMouseInput( &m );
                    break;
                case ButtonPress:
                    this->lb |= event.xbutton.button == x11_window_Button1;
                    this->rb |= event.xbutton.button == x11_window_Button2;
                    m.x = event.xbutton.x;
                    m.y = event.xbutton.y;
                    m.lb = this->lb;
                    m.rb = this->rb;
                    this->addMouseInput( &m );
                    break;
                case ButtonRelease:
                    this->lb &= event.xbutton.button != x11_window_Button1;
                    this->rb &= event.xbutton.button != x11_window_Button2;
                    m.x = event.xbutton.x;
                    m.y = event.xbutton.y;
                    m.lb = this->lb;
                    m.rb = this->rb;
                    this->addMouseInput( &m );
                    break;
                case KeyPress:
                    this->processKb( this->x11.XLookupKeysym( &event.xkey, 0 ), 1 );
                    break;
                case KeyRelease:
                    this->processKb( this->x11.XLookupKeysym( &event.xkey, 0 ), 0 );
                    break;
                case ClientMessage:
                    if( (Atom)event.xclient.data.l[0] == this->wm_delete_window )
                        this->bIsOpen = 0;
                    break;
                case SelectionRequest:
                {
                    x11_window_XEvent rpy;
                    x11_window_XSelectionEvent *r = &rpy.xselection;

                    r->type = x11_window_SelectionNotify;
                    r->time = event.xselectionrequest.time;
                    r->target = event.xselectionrequest.target;
                    r->selection = event.xselectionrequest.selection;
                    r->property = x11_window_None;
                    r->requestor = event.xselectionrequest.requestor;
                    r->display = event.xselectionrequest.display;
                    r->serial = event.xselectionrequest.serial;
                    r->send_event = event.xselectionrequest.send_event;

                    if( r->target == this->a_string )
                    {
                        std::string s;
                        if( /*this->getSelectedText( &s, this->bdocut ) && */s.length() )
                        {
                            r->property = this->selprop;
                            this->x11.XChangeProperty( this->dpy, this->win, this->selprop, this->a_string, 8, x11_window_PropModeReplace, (unsigned char *)s.c_str(), (int)s.length() );
                        }
                    }

                    if( this->a_targets && r->target == this->a_targets )
                    {
                        x11_window_Atom supported[]={ this->a_string };
                        r->property = this->selprop;
                        this->x11.XChangeProperty( this->dpy, this->win, this->selprop, this->a_targets, 8, x11_window_PropModeReplace, (unsigned char *)&supported, sizeof( supported ) );
                    }

                    this->x11.XSendEvent( this->dpy, this->win, 0, x11_window_PropertyChangeMask, &rpy );
                    break;
                }
                case SelectionNotify:
                    x11_window_Atom act_type;
                    int act_fmt;
                    unsigned long itms_returned, itms_remain;
                    char *buf;

                    if( event.xselection.target != this->a_string || !event.xselection.property )
                        break;

                    //event.xselection.property
                    if( this->x11.XGetWindowProperty( event.xselection.display, event.xselection.requestor, event.xselection.property, 0, 4096, 1, this->a_string, &act_type, &act_fmt, &itms_returned, &itms_remain, (unsigned char **)&buf ) != 0 )
                        break;

                    if( act_fmt == 8 && act_type == this->a_string && itms_returned )
                    {
                        std::string s;
                        s.assign( buf, itms_returned );
                        //this->setSelectedText( &s );
                    }

                    this->x11.XFree( buf );
                    break;
                case SelectionClear:

                    break;
                default:
                    break;
            }

        }
    }

    //returns true if x11_window is open
    bool x11_window::isOpen( void )
    {
        return this->bIsOpen;
    }

    //returns x11_window width
    float x11_window::getWidth( void )
    {
        return this->w;
    }

    //returns x11_window height
    float x11_window::getHeight( void )
    {
        return this->h;
    }

    //return x11 window
    x11_window_Window x11_window::getWindow( void )
    {
        return this->win;
    }

    //return screen
    int x11_window::getScreen( void )
    {
        return this->screen;
    }

    //return display
    x11_window_Display *x11_window::getDisplay( void )
    {
        return this->dpy;
    }

    //return vi
    x11_window_XVisualInfo *x11_window::getVi( void )
    {
        return this->vi;
    }

    //process keyboard input
    void x11_window::processKb( KeySym k, bool isDown )
    {
        unsigned char c[ 2 ];
        std::string s;
        window_kb_input m;

        //http://tronche.com/gui/x/icccm/sec-2.html#s-2.1
        //https://en.wikipedia.org/wiki/ASCII

        if( this->bctrl && k == 99 )//ctrl c
        {
            this->x11.XSetSelectionOwner( this->dpy, this->a_primary, this->win, x11_window_CurrentTime );
            this->bdocut = 0;
            return;
        }
        if( this->bctrl && k == XK_X )
        {
            this->x11.XSetSelectionOwner( this->dpy, this->a_primary, this->win, x11_window_CurrentTime );
            this->bdocut = 1;
            return;
        }
        if( this->bctrl && k == 118 ) //ctrl+v
        {
            this->x11.XConvertSelection( this->dpy, this->a_primary, this->a_string, None, this->win, x11_window_CurrentTime );
            return;
        }

        if( k >= 32 && k <= 127 )
        {
            c[ 1 ] = 0;

            if( !this->bshift && !this->bcaps )
            {
                c[ 0 ] = (unsigned char)k;
                s.assign( (const char *)c );
            }
            else
            {
                if( this->bshift )
                {
                    switch( k )
                    {
                        case 49: //1 to !
                            k = 33;
                            break;
                        case 50: //2 to @
                            k = 64;
                            break;
                        case 51: //3 to #
                            k = 35;
                            break;
                        case 52: //4 to $
                            k = 36;
                            break;
                        case 53: //5 to %
                            k = 37;
                            break;
                        case 54: //6 to ^
                            k = 94;
                            break;
                        case 55: //7 to &
                            k = 38;
                            break;
                        case 56: //8 to *
                            k = 42;
                            break;
                        case 57: //9 to (
                            k = 40;
                            break;
                        case 48: //0 to )
                            k = 41;
                            break;
                        case 45: //- to _
                            k = 95;
                            break;
                        case 61: //= to +
                            k = 43;
                            break;
                        case 91: //[ to {
                            k = 123;
                            break;
                        case 93: //] to }
                            k = 125;
                            break;
                        case 92: //\ to |
                            k = 124;
                            break;
                        case 59: //; to :
                            k = 58;
                            break;
                        case 39: //' to "
                            k = 34;
                            break;
                        case 44: //, to <
                            k = 60;
                            break;
                        case 46: //. to >
                            k = 62;
                            break;
                        case 47: /// to ?
                            k = 63;
                            break;
                        default:
                            break;
                    }
                }

                if( k >= 97 && k <= 122 && !( this->bshift && this->bcaps ) )
                    k -= 97 - 65;
                c[ 0 ] = (unsigned char)k;
                s.assign( (const char *)c );
            }
        }
        else
        {

            switch( k )
            {
                case XK_Caps_Lock:
                    s.assign( "Caps Lock" );
                    if( isDown )
                        this->bcaps = !this->bcaps;
                    break;
                case XK_Control_L:
                case XK_Control_R:
                    s.assign( "Control" );
                    this->bctrl = isDown;
                    break;
                case XK_Shift_L:
                case XK_Shift_R:
                    s.assign( "Shift" );
                    this->bshift = isDown;
                    break;
                case XK_BackSpace:
                    s.assign( "Backspace" );
                    break;
                case XK_Tab:
                    s.assign( "Tab" );
                    break;
                case XK_Return:
                    s.assign( "Enter" );
                    break;
                case XK_Escape:
                    s.assign( "Escape" );
                    break;
                case XK_Delete:
                    s.assign( "Delete" );
                    break;
                case XK_KP_Space:
                    s.assign( "Space" );
                    break;
                case XK_KP_Tab:
                    s.assign( "Tab" );
                    break;
                case XK_KP_Enter:
                    s.assign( "Enter" );
                    break;
                case XK_Home:
                case XK_KP_Home:
                    s.assign( "Home" );
                    break;
                case XK_Left:
                case XK_KP_Left:
                    s.assign( "Left" );
                    break;
                case XK_Up:
                case XK_KP_Up:
                    s.assign( "Up" );
                    break;
                case XK_Right:
                case XK_KP_Right:
                    s.assign( "Right" );
                    break;
                case XK_Down:
                case XK_KP_Down:
                    s.assign( "Down" );
                    break;
                case XK_Page_Up:
                case XK_KP_Page_Up:
                    s.assign( "Page Up" );
                    break;
                case XK_Page_Down:
                case XK_KP_Page_Down:
                    s.assign( "Page Down" );
                    break;
                case XK_End:
                case XK_KP_End:
                    s.assign( "End" );
                    break;
                case XK_Begin:
                case XK_KP_Begin:
                    s.assign( "Home" );
                    break;
                case XK_KP_Insert:
                    s.assign( "Insert" );
                    break;
                case XK_KP_Delete:
                    s.assign( "Delete" );
                    break;
                case XK_KP_Equal:
                    s.assign( "=" );
                    break;
                case XK_KP_Multiply:
                    s.assign( "*" );
                    break;
                case XK_KP_Add:
                    s.assign( "+" );
                    break;
                case XK_KP_Separator:
                    s.assign( "," );
                    break;
                case XK_KP_Subtract:
                    s.assign( "-" );
                    break;
                case XK_KP_Decimal:
                    s.assign( "." );
                    break;
                case XK_KP_Divide:
                    s.assign( "/" );
                    break;
                default:
                    c[ 1 ] = 0;

                    //numpad 0 - 9
                    if( k >= XK_KP_0 && k <= XK_KP_9 )
                    {
                        c[ 0 ] = k - XK_KP_0 + 48;
                        s.assign( (const char *)c );
                    }

                    //F1 - F9
                    if( k >= XK_F1 && k <= XK_F9 )
                    {
                        c[ 0 ] = k - XK_F1 + 49;
                        s.assign( "F" );
                        s.append( (const char *)c );
                    }

                    //F10 - F19
                    if( k >= XK_F10 && k <= XK_F19 )
                    {
                        c[ 0 ] = k - XK_F10 + 48;
                        s.assign( "F1" );
                        s.append( (const char *)c );
                    }
            }
        }

        if( s.size() < 1 )
            return;
        m.sname.assign( s );
        m.bDown = isDown;
        this->addKBInput( &m );
    }

};
