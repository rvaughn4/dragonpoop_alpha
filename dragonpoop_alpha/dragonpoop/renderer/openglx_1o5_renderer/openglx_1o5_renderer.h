
#ifndef dragonpoop_openglx_1o5_renderer_h
#define dragonpoop_openglx_1o5_renderer_h

#include "../renderer.h"
#include <stdio.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include "../../gfx/dpmatrix/dpmatrix.h"
#include <vector>

namespace dragonpoop
{

    class openglx_1o5_renderer_readlock;
    class openglx_1o5_renderer_writelock;
    class openglx_1o5_renderer_ref;
    class renderer_model_material;

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
        float fps;
        bool lb, rb;
        uint64_t t_last_motion;
        
        //draw vb
        void drawVb( dpvertexindex_buffer *vb );
        //draw vb
        void drawVb( dpvertex_buffer *vb, std::vector<uint16_t> *indicies );
        //draw vb
        void drawVb( dpvertex *vb, unsigned int vb_size, char *ib, unsigned int ib_size, unsigned int ib_stride );
        //process keyboard input
        void processKb( KeySym k, bool isDown );
        
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
        virtual bool runApi( dpthread_lock *thd );
        //create gl window
        bool makeWindow( char* title, int width, int height, int bits, bool fullscreenflag );
        //kill gl window
        void killWindow( void );
        //run window
        void runWindow( dpthread_lock *thd );
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
        virtual void prepareGuiRender( unsigned int w, unsigned int h );
        //flip backbuffer and present scene to screen
        virtual void flipBuffer( void );
        //generate renderer model
        virtual renderer_model *genModel( model_writelock *ml );
        //generate renderer gui
        virtual renderer_gui *genGui( gui_writelock *ml );
        //render model instance group
        virtual void renderGroup( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world );
        //render gui
        virtual void renderGui( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world );

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
