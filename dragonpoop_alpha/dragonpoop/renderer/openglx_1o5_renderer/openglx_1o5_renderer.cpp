
#include "openglx_1o5_renderer.h"
#include "openglx_1o5_renderer_readlock.h"
#include "openglx_1o5_renderer_writelock.h"
#include "openglx_1o5_renderer_ref.h"
#include "../../core/core.h"
#include "openglx_1o5_renderer_gui/openglx_1o5_renderer_gui.h"
#include "openglx_1o5_renderer_gui/openglx_1o5_renderer_gui_readlock.h"
#include "openglx_1o5_renderer_model/openglx_1o5_renderer_model.h"
#include "openglx_1o5_renderer_model/openglx_1o5_renderer_model_instance/openglx_1o5_renderer_model_group_instance/openglx_1o5_renderer_model_group_instance.h"
#include "openglx_1o5_renderer_model/openglx_1o5_renderer_model_material/openglx_1o5_renderer_model_material.h"
#include "../renderer_model/renderer_model_material/renderer_model_material.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "../../gfx/model/model_instance/model_instance_joint_cache/model_instance_joint_cache.h"
#include "../renderer_model/renderer_model_instance/renderer_model_instance_readlock.h"
#include "../renderer_model/renderer_model_instance/renderer_model_joint_instance/renderer_model_joint_instance.h"
#include "../renderer_model/renderer_model_readlock.h"
#include "../../core/shared_obj/shared_obj_guard.h"

#include <sstream>
#include <vector>
#include <string.h>
#include <math.h>

namespace dragonpoop
{

    //ctor
    openglx_1o5_renderer::openglx_1o5_renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp ) : renderer( c, g, tp )
    {
        memset( &this->gl, 0, sizeof( this->gl ) );
        this->fps = 0;
        this->lb = this->rb = 0;
        this->bshift = this->bctrl = this->bcaps = 0;
    }

    //dtor
    openglx_1o5_renderer::~openglx_1o5_renderer( void )
    {
        shared_obj_guard o;
        
        this->_kill();

        o.tryWriteLock( this, 5000, "openglx_1o5_renderer::~openglx_1o5_renderer" );
        o.unlock();
        this->unlink();
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
    bool openglx_1o5_renderer::runApi( renderer_writelock *r, dpthread_lock *thd )
    {
        this->runWindow( r, thd );
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
        this->gl.attr.event_mask = PropertyChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask;
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
        
        //selection property
        this->gl.selprop = XInternAtom( this->gl.dpy, "SELECTION_PROPERTY", True );
        this->gl.a_targets = XInternAtom( this->gl.dpy, "TARGETS", 1 );
        this->gl.a_clipboard = XInternAtom( this->gl.dpy, "CLIPBOARD", 1 );

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

        glEnable(GL_LIGHTING);
        GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat LightPosition[]= { 0.0f, 0.0f, 20.0f, -10.0f };

        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);

        GLfloat LightAmbient2[]= { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat LightDiffuse2[]= { 1.0f, 0.8f, 0.3f, 1.0f };
        GLfloat LightPosition2[]= { 0.0f, 0.0f, 10.0f, -5.0f };

        glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient2);             // Setup The Ambient Light
        glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse2);             // Setup The Diffuse Light
        glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);
        glEnable(GL_LIGHT2);

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
    void openglx_1o5_renderer::runWindow( renderer_writelock *r, dpthread_lock *thd )
    {
        XEvent event;
        uint64_t t;

        if( this->fps != this->getFps() )
        {
            std::stringstream ss;

            this->fps = this->getFps();
            ss << "Dragonpoop: its as smooth as silk! " << (int)this->fps << " fps (" << this->getMsPerFrame() << " ms)";

            XSetStandardProperties( this->gl.dpy, this->gl.win, ss.str().c_str(), ss.str().c_str(), None, NULL, 0, NULL );
        }

        if( XPending( this->gl.dpy ) < 1 )
            return;

        XNextEvent( this->gl.dpy, &event );
        switch (event.type)
        {
            case ConfigureNotify:
                if(
                   ( event.xconfigure.width != (int)this->gl.width )
                   ||
                   ( event.xconfigure.height != (int)this->gl.height )
                   )
                {
                    this->gl.width = event.xconfigure.width;
                    this->gl.height = event.xconfigure.height;
                }
                break;
            case MotionNotify:
                t = thd->getTicks();
                if( t - this->t_last_motion > 30 )
                {
                    this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                    this->t_last_motion = t;
                }
                break;
            case ButtonPress:
                this->lb |= event.xbutton.button == Button1;
                this->rb |= event.xbutton.button == Button2;
                this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                break;
            case ButtonRelease:
                this->lb &= event.xbutton.button != Button1;
                this->rb &= event.xbutton.button != Button2;
                this->processMouseInput( r, event.xbutton.x, event.xbutton.y, this->lb, this->rb );
                break;
            case KeyPress:
                this->processKb( XLookupKeysym( &event.xkey, 0 ), 1 );
                break;
            case KeyRelease:
                this->processKb( XLookupKeysym( &event.xkey, 0 ), 0 );
                break;
            case ClientMessage:
                if( (Atom)event.xclient.data.l[0] == this->gl.wm_delete_window )
                    this->getCore()->kill();
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
                    if( this->getSelectedText( &s, this->bdocut ) && s.length() )
                    {
                        r->property = this->gl.selprop;
                        XChangeProperty( this->gl.dpy, this->gl.win, this->gl.selprop, XA_STRING, 8, PropModeReplace, (unsigned char *)s.c_str(), (int)s.length() );
                    }
                }

                if( this->gl.a_targets && r->target == this->gl.a_targets )
                {
                    Atom supported[]={ XA_STRING };
                    r->property = this->gl.selprop;
                    XChangeProperty( this->gl.dpy, this->gl.win, this->gl.selprop, this->gl.a_targets, 8, PropModeReplace, (unsigned char *)&supported, sizeof( supported ) );
                }
            
                XSendEvent( this->gl.dpy, this->gl.win, 0, PropertyChangeMask, &rpy );
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
                    this->setSelectedText( &s );
                }
                
                XFree( buf );
                break;
            case SelectionClear:
                
                break;
            default:
                break;
        }

    }

    //get renderer name
    void openglx_1o5_renderer::getName( std::string *s )
    {
        s->assign( "X11 OpenGL 1.5" );
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
        this->renderer::prepareWorldRender( w, h );

        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT );
        glEnable( GL_LIGHTING );
        glEnable( GL_DEPTH_TEST );
    }

    //prepare for rendering gui
    void openglx_1o5_renderer::prepareGuiRender( unsigned int w, unsigned int h )
    {
        this->renderer::prepareGuiRender( w, h );
        glClearDepth( 1.0f );
        
        glClear( GL_DEPTH_BUFFER_BIT );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glEnable( GL_BLEND );
        glDisable( GL_LIGHTING );
        glDisable( GL_DEPTH_TEST );
    }

    //flip backbuffer and present scene to screen
    void openglx_1o5_renderer::flipBuffer( void )
    {
        glXSwapBuffers( this->gl.dpy, this->gl.win );
    }

    //generate renderer model
    renderer_model *openglx_1o5_renderer::genModel( model_writelock *ml )
    {
        return new openglx_1o5_renderer_model( ml, this );
    }
    
    //generate renderer gui
    renderer_gui *openglx_1o5_renderer::genGui( gui_writelock *ml )
    {
        return new openglx_1o5_renderer_gui( ml, this );
    }

    //draw vb
    void openglx_1o5_renderer::drawVb( dpvertexindex_buffer *vb )
    {
        dpvertex *p_vb;
        unsigned int vb_size;
        dpindex *p_ib;
        unsigned int ib_size;
        
        p_vb = vb->getVertexBuffer( &vb_size );
        p_ib = vb->getIndexBuffer( &ib_size );
        
        this->drawVb( p_vb, vb_size, (char *)&p_ib->i, ib_size, sizeof( dpindex ) );
    }
    
    //draw vb
    void openglx_1o5_renderer::drawVb( dpvertex_buffer *vb, std::vector<uint16_t> *indicies )
    {
        dpvertex *p_vb;
        unsigned int vb_size;

        p_vb = vb->getBuffer();
        vb_size = vb->getSize();
        
        this->drawVb( p_vb, vb_size, (char *)&( *indicies )[ 0 ], (unsigned int)indicies->size(), sizeof( uint16_t ) );
    }
    
    //draw vb
    void openglx_1o5_renderer::drawVb( dpvertex *vb, unsigned int vb_size, char *ib, unsigned int ib_size, unsigned int ib_stride )
    {
        dpvertex *v;
        uint16_t in;
        unsigned int i;
        
        glBegin( GL_TRIANGLES );
        
        for( i = 0; i < ib_size; i++ )
        {
            in = *( (uint16_t *)ib );
            ib += ib_stride;
            
            if( in >= vb_size )
                continue;
            v = &vb[ in ];
            
            glTexCoord2f( v->texcoords[ 0 ].s, v->texcoords[ 0 ].t );
            glNormal3f( v->normal.x, v->normal.y, v->normal.z );
            glVertex3f( v->pos.x, v->pos.y, v->pos.z );
        }
        
        glEnd();
    }
    
    //render model instance group
    void openglx_1o5_renderer::renderGroup( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world )
    {
        openglx_1o5_renderer_model_material *gmat;
        dpvertex_buffer *vb;
        std::vector<uint16_t> *indicies;
        dpmatrix wmat;
        uint64_t t, ot;

        mi->getModelViewMatrix( thd, r, m, m_world, &wmat );
        glLoadMatrixf( wmat.getRaw4by4() );

        gmat = (openglx_1o5_renderer_model_material *)mat;
        glBindTexture( GL_TEXTURE_2D, gmat->getDiffuseTex() );
        
        t = thd->getTicks();
        ot = ( (openglx_1o5_renderer_model_instance_group *)g )->getLastFrameTime();

        mi->redoMatrixes( thd->getTicks() );
        ( (openglx_1o5_renderer_model_instance_group *)g )->setLastFrameTime( t );
            
        vb = g->getTransformedBuffer( mi, &indicies );
        this->drawVb( vb, indicies );
    }
    
    //render gui
    void openglx_1o5_renderer::renderGui( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world )
    {
        float a;
        
        glLoadMatrixf( m_world->getRaw4by4() );

        a = m->getOpacity();

        if( m->hasBg() && ( (openglx_1o5_renderer_gui_readlock *)m )->getBgTex() )
        {
            glColor4f( 1, 1, 1, a * 0.9f );
            glBindTexture( GL_TEXTURE_2D, ( (openglx_1o5_renderer_gui_readlock *)m )->getBgTex() );
            drawVb( m->getBgBuffer() );
        }
        
        if( m->hasFg() && ( (openglx_1o5_renderer_gui_readlock *)m )->getFgTex() )
        {
            glColor4f( 1, 1, 1, a );
            glBindTexture( GL_TEXTURE_2D, ( (openglx_1o5_renderer_gui_readlock *)m )->getFgTex() );
            drawVb( m->getFgBuffer() );
        }
    }
    
    //process keyboard input
    void openglx_1o5_renderer::processKb( KeySym k, bool isDown )
    {
        unsigned char c[ 2 ];
        std::string s;
        
        //http://tronche.com/gui/x/icccm/sec-2.html#s-2.1
        //https://en.wikipedia.org/wiki/ASCII
        
        if( this->bctrl && k == 99 )//ctrl c
        {
            XSetSelectionOwner( this->gl.dpy, XA_PRIMARY, this->gl.win, CurrentTime );
            this->bdocut = 0;
            return;
        }
        if( this->bctrl && k == XK_X )
        {
            XSetSelectionOwner( this->gl.dpy, XA_PRIMARY, this->gl.win, CurrentTime );
            this->bdocut = 1;
            return;
        }
        if( this->bctrl && k == 118 ) //ctrl+v
        {
            XConvertSelection( this->gl.dpy, XA_PRIMARY, XA_STRING, None, this->gl.win, CurrentTime );
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
        
        if( s.size() > 0 )
            this->renderer::processKbInput( &s, isDown );
    }
    
    //add texture memory
    void openglx_1o5_renderer::addTexMemory( unsigned int m )
    {
        this->tex_mem += m;
    }
    
    //subtract texture memory
    void openglx_1o5_renderer::subTexMemory( unsigned int m )
    {
        this->tex_mem -= m;
    }
    
    //get texture memory used
    unsigned int openglx_1o5_renderer::getTextureMemory( void )
    {
        return this->tex_mem;
    }
    
};
