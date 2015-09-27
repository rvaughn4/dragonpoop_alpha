
#ifndef dragonpoop_renderer_writelock_h
#define dragonpoop_renderer_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"
#include <list>
#include "../core/dpid/dpid.h"

namespace dragonpoop
{
    class renderer;
    class core;
    class dpthread_lock;
    class dptask_writelock;
    class gfx_writelock;
    class renderer_writelock;
    class renderer_model_readlock;
    class renderer_model_instance_readlock;
    class renderer_model_instance_group;
    class renderer_model_material;
    class dpmatrix;
    class renderer_gui_readlock;
    class renderer_gui;

    class renderer_writelock : public shared_obj_writelock
    {

    private:

        renderer *t;

    protected:

        //ctor
        renderer_writelock( renderer *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_writelock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //stop renderer task
        void kill( void );
        //run renderer from task
        void run( dptask_writelock *tskl, dpthread_lock *thd );
        //render model instance group
        void renderGroup( dpthread_lock *thd, renderer_model_readlock *m, renderer_model_instance_readlock *mi, renderer_model_instance_group *g, renderer_model_material *mat, dpmatrix *m_world );
        //render gui
        void renderGui( dpthread_lock *thd, renderer_gui_readlock *m, dpmatrix *m_world );
        //returns fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsPerFrame( void );
        //return guis
        void getChildrenGuis( std::list<renderer_gui *> *l, dpid pid );
        //get hovering gui id
        dpid getHoverId( void );

        friend class renderer;
    };
    
};

#endif