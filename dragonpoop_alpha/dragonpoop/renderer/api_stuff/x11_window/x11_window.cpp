
#include "x11_window.h"
#include "../window/window.h"
#include <string>

namespace dragonpoop
{

    //ctor
    x11_window::x11_window( float w, float h, const char *ctitle ) : window( w, h, ctitle )
    {
        Colormap cmap;

        //open display
        XInitThreads();
        this->bIsOpen = 0;
        this->dpy = XOpenDisplay( 0 );
        this->win = 0;
        this->screen = DefaultScreen( this->dpy );

        //set visual format
        static int attrListSgl[] =
        {
            GLX_RGBA,
            GLX_RED_SIZE, 4,
            GLX_GREEN_SIZE, 4,
            GLX_BLUE_SIZE, 4,
            GLX_DEPTH_SIZE, 16,
            None
        };
        static int attrListDbl[] =
        {
            GLX_RGBA, GLX_DOUBLEBUFFER,
            GLX_RED_SIZE, 4,
            GLX_GREEN_SIZE, 4,
            GLX_BLUE_SIZE, 4,
            GLX_DEPTH_SIZE, 16,
            None
        };
        vi = glXChooseVisual( this->dpy, this->screen, attrListDbl );
        if( !vi )
            vi = glXChooseVisual( this->dpy, this->screen, attrListSgl );
        if( !vi )
        {
            XCloseDisplay( this->dpy );
            this->dpy = 0;
            return;
        }

        //colormap
        cmap = XCreateColormap( this->dpy, RootWindow( this->dpy, vi->screen ), vi->visual, AllocNone );
        this->attr.colormap = cmap;
        this->attr.border_pixel = 0;

        //create window
        this->attr.event_mask = PropertyChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask;
        this->win = XCreateWindow( this->dpy, RootWindow( this->dpy, vi->screen ), 0, 0, w, h, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &this->attr );
        if( !this->win )
        {
            XCloseDisplay( this->dpy );
            this->dpy = 0;
            return;
        }

        //setup close button event
        this->wm_delete_window = XInternAtom( this->dpy, "WM_DELETE_WINDOW", True );
        XSetWMProtocols( this->dpy, this->win, &this->wm_delete_window, 1 );

        //selection property
        this->selprop = XInternAtom( this->dpy, "SELECTION_PROPERTY", True );
        this->a_targets = XInternAtom( this->dpy, "TARGETS", 1 );
        this->a_clipboard = XInternAtom( this->dpy, "CLIPBOARD", 1 );

        //set title
        XSetStandardProperties( this->dpy, this->win, ctitle, ctitle, None, NULL, 0, NULL );
        XMapRaised( this->dpy, this->win );

        this->bIsOpen = 1;
    }

    //dtor
    x11_window::~x11_window( void )
    {
        if( this->dpy )
        {
            XDestroyWindow( this->dpy, this->win );
            XCloseDisplay( this->dpy );
        }
    }

    //run x11_window
    void x11_window::run( void )
    {
        XEvent event;
        window_mouse_input m;

        while( XPending( this->dpy ) >= 1 )
        {

            XNextEvent( this->dpy, &event );
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
                    this->lb |= event.xbutton.button == Button1;
                    this->rb |= event.xbutton.button == Button2;
                    m.x = event.xbutton.x;
                    m.y = event.xbutton.y;
                    m.lb = this->lb;
                    m.rb = this->rb;
                    this->addMouseInput( &m );
                    break;
                case ButtonRelease:
                    this->lb &= event.xbutton.button != Button1;
                    this->rb &= event.xbutton.button != Button2;
                    m.x = event.xbutton.x;
                    m.y = event.xbutton.y;
                    m.lb = this->lb;
                    m.rb = this->rb;
                    this->addMouseInput( &m );
                    break;
                case KeyPress:
                    this->processKb( XLookupKeysym( &event.xkey, 0 ), 1 );
                    break;
                case KeyRelease:
                    this->processKb( XLookupKeysym( &event.xkey, 0 ), 0 );
                    break;
                case ClientMessage:
                    if( (Atom)event.xclient.data.l[0] == this->wm_delete_window )
                        this->bIsOpen = 0;
                    break;
                case SelectionRequest:
                {
                    XEvent rpy;
                    XSelectionEvent *r = &rpy.xselection;

                    r->type = SelectionNotify;
                    r->time = event.xselectionrequest.time;
                    r->target = event.xselectionrequest.target;
                    r->selection = event.xselectionrequest.selection;
                    r->property = None;
                    r->requestor = event.xselectionrequest.requestor;
                    r->display = event.xselectionrequest.display;
                    r->serial = event.xselectionrequest.serial;
                    r->send_event = event.xselectionrequest.send_event;

                    if( r->target == XA_STRING )
                    {
                        std::string s;
                        if( /*this->getSelectedText( &s, this->bdocut ) && */s.length() )
                        {
                            r->property = this->selprop;
                            XChangeProperty( this->dpy, this->win, this->selprop, XA_STRING, 8, PropModeReplace, (unsigned char *)s.c_str(), (int)s.length() );
                        }
                    }

                    if( this->a_targets && r->target == this->a_targets )
                    {
                        Atom supported[]={ XA_STRING };
                        r->property = this->selprop;
                        XChangeProperty( this->dpy, this->win, this->selprop, this->a_targets, 8, PropModeReplace, (unsigned char *)&supported, sizeof( supported ) );
                    }

                    XSendEvent( this->dpy, this->win, 0, PropertyChangeMask, &rpy );
                    break;
                }
                case SelectionNotify:
                    Atom act_type;
                    int act_fmt;
                    unsigned long itms_returned, itms_remain;
                    char *buf;

                    if( event.xselection.target != XA_STRING || !event.xselection.property )
                        break;

                    //event.xselection.property
                    if( XGetWindowProperty( event.xselection.display, event.xselection.requestor, event.xselection.property, 0, 4096, 1, XA_STRING, &act_type, &act_fmt, &itms_returned, &itms_remain, (unsigned char **)&buf ) != Success )
                        break;

                    if( act_fmt == 8 && act_type == XA_STRING && itms_returned )
                    {
                        std::string s;
                        s.assign( buf, itms_returned );
                        //this->setSelectedText( &s );
                    }

                    XFree( buf );
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
    Window x11_window::getWindow( void )
    {
        return this->win;
    }

    //return screen
    int x11_window::getScreen( void )
    {
        return this->screen;
    }

    //return display
    Display *x11_window::getDisplay( void )
    {
        return this->dpy;
    }

    //return vi
    XVisualInfo *x11_window::getVi( void )
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
            XSetSelectionOwner( this->dpy, XA_PRIMARY, this->win, CurrentTime );
            this->bdocut = 0;
            return;
        }
        if( this->bctrl && k == XK_X )
        {
            XSetSelectionOwner( this->dpy, XA_PRIMARY, this->win, CurrentTime );
            this->bdocut = 1;
            return;
        }
        if( this->bctrl && k == 118 ) //ctrl+v
        {
            XConvertSelection( this->dpy, XA_PRIMARY, XA_STRING, None, this->win, CurrentTime );
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
