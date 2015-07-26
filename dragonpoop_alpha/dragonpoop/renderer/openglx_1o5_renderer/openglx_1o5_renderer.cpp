
#include "openglx_1o5_renderer.h"
#include "openglx_1o5_renderer_readlock.h"
#include "openglx_1o5_renderer_writelock.h"
#include "openglx_1o5_renderer_ref.h"
#include "../../core/core.h"

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer::openglx_1o5_renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : renderer( c, g, tp )
    {
        memset( &this->gl, 0, sizeof( this->gl ) );
    }

    //dtor
    openglx_1o5_renderer::~openglx_1o5_renderer( void )
    {
        this->_kill();
    }

    //generate read lock
    shared_obj_readlock *openglx_1o5_renderer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new openglx_1o5_renderer_readlock( (openglx_1o5_renderer *)p, l );
    }

    //generate write lock
    shared_obj_writelock *openglx_1o5_renderer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new openglx_1o5_renderer_writelock( (openglx_1o5_renderer *)p, l );
    }

    //generate ref
    shared_obj_ref *openglx_1o5_renderer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new openglx_1o5_renderer_ref( (openglx_1o5_renderer *)p, k );
    }

    //init graphics api
    bool openglx_1o5_renderer::initApi( void )
    {
        if( !this->makeWindow( (char *)"Dragonpoop: its as smooth as silk!", 1024, 728, 32, 0 ) )
            return 0;

        return 1;
    }

    //deinit graphics api
    void openglx_1o5_renderer::deinitApi( void )
    {
        this->killWindow();
    }

    //do background graphics api processing
    bool openglx_1o5_renderer::runApi( void )
    {
        this->runWindow();
        return 1;
    }

    //create gl window
    bool openglx_1o5_renderer::makeWindow( char* title, int width, int height, int bits, bool fullscreenflag )
    {
        XVisualInfo *vi;
        Colormap cmap;
        Window winDummy;
        unsigned int borderDummy;

        this->gl.fs = fullscreenflag;

        //open display
        this->gl.dpy = XOpenDisplay( 0 );
        this->gl.screen = DefaultScreen( this->gl.dpy );

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
        vi = glXChooseVisual( this->gl.dpy, this->gl.screen, attrListDbl );
        this->gl.doubleBuffered = vi != 0;
        if( !vi )
            vi = glXChooseVisual(this->gl.dpy, this->gl.screen, attrListSgl );
        if( !vi )
        {
            XCloseDisplay( this->gl.dpy );
            this->gl.dpy = 0;
            return 0;
        }

        //create context
        this->gl.ctx = glXCreateContext( this->gl.dpy, vi, 0, GL_TRUE );
        if( !this->gl.ctx )
        {
            XCloseDisplay( this->gl.dpy );
            this->gl.dpy = 0;
            return 0;
        }

        //colormap
        cmap = XCreateColormap( this->gl.dpy, RootWindow( this->gl.dpy, vi->screen ), vi->visual, AllocNone );
        this->gl.attr.colormap = cmap;
        this->gl.attr.border_pixel = 0;

        //create window
        this->gl.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
        this->gl.win = XCreateWindow( this->gl.dpy, RootWindow( this->gl.dpy, vi->screen ), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &this->gl.attr );
        if( !this->gl.win )
        {
            glXDestroyContext( this->gl.dpy, this->gl.ctx );
            XCloseDisplay( this->gl.dpy );
            this->gl.dpy = 0;
            this->gl.ctx = 0;
            return 0;
        }

        //setup close button event
        this->gl.wm_delete_window = XInternAtom( this->gl.dpy, "WM_DELETE_WINDOW", True );
        XSetWMProtocols( this->gl.dpy, this->gl.win, &this->gl.wm_delete_window, 1 );

        //set title
        XSetStandardProperties( this->gl.dpy, this->gl.win, title, title, None, NULL, 0, NULL );
        XMapRaised( this->gl.dpy, this->gl.win );

        //male context active
        glXMakeCurrent( this->gl.dpy, this->gl.win, this->gl.ctx );

        //get window position
        XGetGeometry( this->gl.dpy, this->gl.win, &winDummy, &this->gl.x, &this->gl.y, &this->gl.width, &this->gl.height, &borderDummy, &this->gl.depth );

        glDisable(GL_CULL_FACE);

        glShadeModel( GL_SMOOTH );
        glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
        glClearDepth( 1.0f );
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glEnable( GL_TEXTURE_2D );

        return 1;
    }

    //kill gl window
    void openglx_1o5_renderer::killWindow( void )
    {
        if( this->gl.dpy && this->gl.ctx )
        {
            glXMakeCurrent( this->gl.dpy, None, 0 );
            glXDestroyContext( this->gl.dpy, this->gl.ctx );
        }
        this->gl.ctx = 0;
        if( this->gl.dpy )
            XCloseDisplay( this->gl.dpy );
        this->gl.dpy = 0;
    }

    //run window
    void openglx_1o5_renderer::runWindow( void )
    {
        XEvent event;

        if( XPending( this->gl.dpy ) < 1 )
            return;

        XNextEvent( this->gl.dpy, &event );
        switch (event.type)
        {
            case Expose:
                if( event.xexpose.count != 0 )
                    break;
                break;
            case ConfigureNotify:
                if(
                   ( event.xconfigure.width != this->gl.width )
                   ||
                   ( event.xconfigure.height != this->gl.height )
                   )
                {
                    this->gl.width = event.xconfigure.width;
                    this->gl.height = event.xconfigure.height;
                }
                break;
            case ButtonPress:
                break;
            case KeyPress:
                if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
                {
                    // done = True;
                }
                if (XLookupKeysym(&event.xkey,0) == XK_F1)
                {
                    // killGLWindow();
                    // GLWin.fs = !GLWin.fs;
                    //createGLWindow("NeHe's OpenGL Framework", 640, 480, 24, GLWin.fs);
                }
                break;
            case ClientMessage:
                if( (Atom)event.xclient.data.l[0] == this->gl.wm_delete_window )
                    this->getCore()->kill();
                break;
            default:
                break;
        }

    }

    //return screen/window width
    unsigned int openglx_1o5_renderer::getWidth( void )
    {
        return this->gl.width;
    }

    //return screen/window height
    unsigned int openglx_1o5_renderer::getHeight( void )
    {
        return this->gl.height;
    }

    //set viewport size
    void openglx_1o5_renderer::setViewport( unsigned int w, unsigned int h )
    {
        glViewport( 0, 0, w, h );
    }

    //clear screen with color
    void openglx_1o5_renderer::clearScreen( float r, float g, float b )
    {
        glClearDepth( 1.0f );
        glClearColor( r, g, b, 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    }

    //prepare for rendering world
    void openglx_1o5_renderer::prepareWorldRender( unsigned int w, unsigned int h )
    {
        float fw, fh;

        fw = w * 0.5f;
        fh = h * 0.5f;

        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT );

        this->world_m.setPerspective( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 100.0f, 45.0f );
    }

    //prepare for rendering gui
    void openglx_1o5_renderer::prepareGuiRender( void )
    {
        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    //flip backbuffer and present scene to screen
    void openglx_1o5_renderer::flipBuffer( void )
    {
        glXSwapBuffers( this->gl.dpy, this->gl.win );
    }

};
