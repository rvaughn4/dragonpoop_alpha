
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

        this->makeBox();
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
        this->gl.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | VisibilityChangeMask | FocusChangeMask;
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

        glEnableClientState( GL_NORMAL_ARRAY );
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
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
            case VisibilityNotify:
                //this->setActiveState( event.xvisibility.state == VisibilityUnobscured );
                break;
            case FocusIn:
                //this->setActiveState( 1 );
                break;
            case FocusOut:
                //this->setActiveState( 0 );
                break;
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
        this->renderer::prepareWorldRender( w, h );
        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT );

        this->world_m.setPerspective( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 100.0f, 45.0f );
    }

    //prepare for rendering gui
    void openglx_1o5_renderer::prepareGuiRender( unsigned int w, unsigned int h )
    {
        this->renderer::prepareGuiRender( w, h );
        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    //flip backbuffer and present scene to screen
    void openglx_1o5_renderer::flipBuffer( void )
    {
        glXSwapBuffers( this->gl.dpy, this->gl.win );
    }

    //generate renderer model
    renderer_model *openglx_1o5_renderer::genModel( model_writelock *ml )
    {
        return new openglx_1o5_renderer_model( ml );
    }
    
    //generate renderer gui
    renderer_gui *openglx_1o5_renderer::genGui( gui_writelock *ml )
    {
        return new openglx_1o5_renderer_gui( ml );
    }

    //render model instance group
    void openglx_1o5_renderer::renderGroup( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world )
    {
        openglx_1o5_renderer_model_material *gmat;
        dpvertex_buffer *vb;
        dpvertex *vp;
        std::vector<uint16_t> *indicies;
        dpmatrix wmat;
        uint64_t t, ot;

        mi->getModelViewMatrix( r, m, m_world, &wmat );
        glLoadMatrixf( wmat.getRaw4by4() );

        gmat = (openglx_1o5_renderer_model_material *)mat;
        glBindTexture( GL_TEXTURE_2D, gmat->getDiffuseTex() );
        
        t = thd->getTicks();
        ot = ( (openglx_1o5_renderer_model_instance_group *)g )->getLastFrameTime();

        mi->redoMatrixes( thd->getTicks() );
        ( (openglx_1o5_renderer_model_instance_group *)g )->setLastFrameTime( t );
            
        vb = g->getTransformedBuffer( mi, &indicies );
        vp = vb->getBuffer();

        glTexCoordPointer( 2, GL_FLOAT, sizeof( dpvertex ), &vp->texcoords[ 0 ] );
        glNormalPointer( GL_FLOAT, sizeof( dpvertex ), &vp->normal );
        glVertexPointer( 3, GL_FLOAT, sizeof( dpvertex ), &vp->pos );
        glDrawElements( GL_TRIANGLES, (int)indicies->size(), GL_UNSIGNED_SHORT, &( *indicies )[ 0 ] );
    }
    
    //render gui
    void openglx_1o5_renderer::renderGui( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world )
    {
        dpvertex *vp;
        
        glLoadMatrixf( m_world->getRaw4by4() );

        vp = this->vbbox.getVertexBuffer( 0 );
        glTexCoordPointer( 2, GL_FLOAT, sizeof( dpvertex ), &vp->texcoords[ 0 ] );
        glNormalPointer( GL_FLOAT, sizeof( dpvertex ), &vp->normal );
        glVertexPointer( 3, GL_FLOAT, sizeof( dpvertex ), &vp->pos );

        if( m->hasBg() && ( (openglx_1o5_renderer_gui_readlock *)m )->getBgTex() )
        {
            glBindTexture( GL_TEXTURE_2D, ( (openglx_1o5_renderer_gui_readlock *)m )->getBgTex() );
            glDrawElements( GL_TRIANGLES, (int)this->vbbox_indicies.size(), GL_UNSIGNED_SHORT, &this->vbbox_indicies[ 0 ] );
        }
        
        if( m->hasFg() && ( (openglx_1o5_renderer_gui_readlock *)m )->getFgTex() )
        {
            glBindTexture( GL_TEXTURE_2D, ( (openglx_1o5_renderer_gui_readlock *)m )->getFgTex() );
            glDrawElements( GL_TRIANGLES, (int)this->vbbox_indicies.size(), GL_UNSIGNED_SHORT, &this->vbbox_indicies[ 0 ] );
        }
    }
    
    //build gui box
    void openglx_1o5_renderer::makeBox( void )
    {
        dpvertex b;
        dpindex *p, *ip;
        unsigned int i, e;
        
        this->vbbox.clear();

        b.normal.x = 0;
        b.normal.y = 0;
        b.normal.z = 1;
        b.pos.z = -1;
        
        //t_0
        
        //tl
        b.pos.x = 0;
        b.pos.y = 0;
        b.texcoords[ 0 ].s = 0;
        b.texcoords[ 0 ].t = 0;
        this->vbbox.addVertexUnique( &b );
        
        //bl
        b.pos.x = 0;
        b.pos.y = 1;
        b.texcoords[ 0 ].s = 0;
        b.texcoords[ 0 ].t = 1;
        this->vbbox.addVertexUnique( &b );
        
        //tr
        b.pos.x = 1;
        b.pos.y = 0;
        b.texcoords[ 0 ].s = 1;
        b.texcoords[ 0 ].t = 0;
        this->vbbox.addVertexUnique( &b );
        
        //t_1
        
        //tr
        b.pos.x = 1;
        b.pos.y = 0;
        b.texcoords[ 0 ].s = 1;
        b.texcoords[ 0 ].t = 0;
        this->vbbox.addVertexUnique( &b );
        
        //bl
        b.pos.x = 0;
        b.pos.y = 1;
        b.texcoords[ 0 ].s = 0;
        b.texcoords[ 0 ].t = 1;
        this->vbbox.addVertexUnique( &b );
        
        //br
        b.pos.x = 1;
        b.pos.y = 1;
        b.texcoords[ 0 ].s = 1;
        b.texcoords[ 0 ].t = 1;
        this->vbbox.addVertexUnique( &b );
        
        this->vbbox_indicies.clear();
        ip = this->vbbox.getIndexBuffer( &e );
        for( i = 0; i < e; i++ )
        {
            p = &ip[ i ];
            this->vbbox_indicies.push_back( p->i );
        }
        
    }
    
};
