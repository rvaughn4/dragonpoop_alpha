
#include "x11_window.h"
#include <string>

namespace dragonpoop
{
    
    //ctor
    x11_window::x11_window( float w, float h, const char *ctitle ) : window( w, h, ctitle )
    {
        Colormap cmap;
        
        //open display
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
        
        if( XPending( this->dpy ) < 1 )
            return;
        
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
                //this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                break;
            case ButtonPress:
                this->lb |= event.xbutton.button == Button1;
                this->rb |= event.xbutton.button == Button2;
               // this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                break;
            case ButtonRelease:
                this->lb &= event.xbutton.button != Button1;
                this->rb &= event.xbutton.button != Button2;
               // this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                break;
            case KeyPress:
                //this->processKb( XLookupKeysym( &event.xkey, 0 ), 1 );
                break;
            case KeyRelease:
                //this->processKb( XLookupKeysym( &event.xkey, 0 ), 0 );
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
    
};
