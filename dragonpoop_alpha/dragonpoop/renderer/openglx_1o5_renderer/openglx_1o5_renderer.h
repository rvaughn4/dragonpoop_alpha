
#ifndef dragonpoop_openglx_1o5_renderer_h
#define dragonpoop_openglx_1o5_renderer_h

#include "../renderer.h"
#include <stdio.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include "../../gfx/dpmatrix/dpmatrix.h"

namespace dragonpoop
{

    class openglx_1o5_renderer_readlock;
    class openglx_1o5_renderer_writelock;
    class openglx_1o5_renderer_ref;

    struct openglx_1o5_stuffs
    {
        Display *dpy;
        int screen;
        Window win;
        GLXContext ctx;
        XSetWindowAttributes attr;
        Bool fs;
        Bool doubleBuffered;
        int x, y;
        unsigned int width, height;
        unsigned int depth;
        Atom wm_delete_window;
    };

    class openglx_1o5_renderer : public renderer
    {

    private:

        openglx_1o5_stuffs gl;
        dpmatrix world_m, local_m;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //init graphics api
        virtual bool initApi( void );
        //deinit graphics api
        virtual void deinitApi( void );
        //do background graphics api processing
        virtual bool runApi( void );
        //create gl window
        bool makeWindow( char* title, int width, int height, int bits, bool fullscreenflag );
        //kill gl window
        void killWindow( void );
        //run window
        void runWindow( void );
        //return screen/window width
        virtual unsigned int getWidth( void );
        //return screen/window height
        virtual unsigned int getHeight( void );
        //set viewport size
        virtual void setViewport( unsigned int w, unsigned int h );
        //clear screen with color
        virtual void clearScreen( float r, float g, float b );
        //prepare for rendering world
        virtual void prepareWorldRender( unsigned int w, unsigned int h );
        //prepare for rendering gui
        virtual void prepareGuiRender( void );
        //flip backbuffer and present scene to screen
        virtual void flipBuffer( void );

    public:

        //ctor
        openglx_1o5_renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~openglx_1o5_renderer( void );

        friend class openglx_1o5_renderer_readlock;
        friend class openglx_1o5_renderer_writelock;
        friend class openglx_1o5_renderer_ref;
    };
    
};

#endif