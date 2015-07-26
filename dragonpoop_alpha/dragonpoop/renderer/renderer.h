
#ifndef dragonpoop_renderer_h
#define dragonpoop_renderer_h

#include "../core/shared_obj/shared_obj.h"

namespace dragonpoop
{

    class dptaskpool_writelock;
    class dptask;
    class renderer_task;
    class dpthread_lock;
    class dptask_writelock;
    class core;
    class gfx_writelock;
    class renderer_writelock;
    class gfx_ref;
    class gfx_writelock;

    class renderer : public shared_obj
    {

    private:

        dptask *tsk;
        renderer_task *gtsk;
        core *c;
        gfx_ref *g;
        std::atomic<bool> bDoRun, bIsRun;

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //stop renderer task
        void kill( void );
        //run renderer from task
        void run( dptask_writelock *tskl, dpthread_lock *thd, renderer_writelock *r );
        //init graphics api
        virtual bool initApi( void );
        //deinit graphics api
        virtual void deinitApi( void );
        //do background graphics api processing
        virtual bool runApi( void );
        //stop task and deinit api
        void _kill( void );
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
        renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~renderer( void );
        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );

        friend class renderer_readlock;
        friend class renderer_writelock;
    };
    
};

#endif