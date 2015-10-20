
#include "opengl1o5_x11.h"

#include <random>

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11::opengl1o5_x11( float w, float h, const char *ctitle, dpmutex_master *mm ) : render_api( new x11_window( w, h, ctitle ), mm )
    {
        Window winDummy;
        unsigned int borderDummy;

        this->w = (x11_window *)this->getWindow();
        this->bIsOpen = this->w->isOpen();
        if( !this->bIsOpen )
            return;
        
        this->dpy = this->w->getDisplay();
        this->screen = this->w->getScreen();
        this->win = this->w->getWindow();
    
        //create context
        this->ctx = glXCreateContext( this->dpy, this->w->getVi(), 0, GL_TRUE );
        if( !this->ctx )
            return;
    
        //male context active
        glXMakeCurrent( this->dpy, this->win, this->ctx );
        
        //get window position
        XGetGeometry( this->dpy, this->win, &winDummy, &this->x, &this->y, &this->width, &this->height, &borderDummy, &this->depth );
        
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
        
        glEnableClientState( GL_NORMAL_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glEnableClientState( GL_VERTEX_ARRAY );
        
        this->bIsOpen = 1;
    }
    
    //dtor
    opengl1o5_x11::~opengl1o5_x11( void )
    {
        if( this->dpy && this->ctx )
        {
            glXMakeCurrent( this->dpy, None, 0 );
            glXDestroyContext( this->dpy, this->ctx );
        }
    }
    
    //run api
    void opengl1o5_x11::run( void )
    {
        this->render_api::run();
        glClearColor( (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1 );
        glClearDepth( 1.0f );
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
        glXSwapBuffers( this->dpy, this->win );
    }
    
    //returns true if window is open
    bool opengl1o5_x11::isOpen( void )
    {
        if( !this->render_api::isOpen() )
            return 0;
        return this->bIsOpen;
    }

};
