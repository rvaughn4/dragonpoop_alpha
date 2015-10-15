
#ifndef dragonpoop_gfx_h
#define dragonpoop_gfx_h

#include "../core/shared_obj/shared_obj.h"
#include "../core/bytetree/dpid_bytetree.h"
#include "dpposition/dpposition.h"

namespace dragonpoop
{

    class dptaskpool_writelock;
    class dptask;
    class gfx_task;
    class dpthread_lock;
    class core;
    class renderer;
    class gfx_writelock;
    class model_ref;
    class model;
    class dptaskpool_ref;
    class model_loader_ref;
    class model_loader;
    class model_instance_ref;
    class model_saver;
    class model_saver_ref;
    class gui;
    class gui_ref;
    class gfx_ref;
    class gui_factory_ref;
    class gui_factory;
    class renderer_ref;
    class dpactor;
    class dpactor_ref;
    class dpland_man;
    class dpactor_man;
    class model_man;
    class model_man_ref;
    class model_man_readlock;
    class model_man_writelock;
    class shared_obj_guard;
    class gui_man;
    class gui_man_ref;
    class gui_man_readlock;
    class gui_man_writelock;
    class dpactor_man_ref;
    class dpactor_man_readlock;
    class dpactor_man_writelock;
    class renderer_factory;

    class gfx : public shared_obj
    {

    private:

        dptask *tsk;
        gfx_task *gtsk;
        dpland_man *land_mgr;
        dpactor_man *actor_mgr;
        model_man *model_mgr;
        gui_man *gui_mgr;
        
        core *c;
        std::list<renderer_factory *> renderer_factories;
        renderer *r;
        dptaskpool_ref *tpr;
        unsigned int ms_each_frame, model_cnt, gui_cnt;
        float fps;
        uint64_t last_r_poll;
        dpposition cam_pos;
        bool bIsRun;
        
        gui_factory_ref *root_factory;
        gui *root_g;
        
        //start all tasks
        void _startTask( core *c, dptaskpool_writelock *tp );
        //kill all tasks
        void _killTask( void );
        //delete all tasks
        void _deleteTask( void );
        
    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //stop gfx task
        void kill( void );
        //run gfx from task
        void run( dpthread_lock *thd, gfx_writelock *g );
        //return fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsEachFrame( void );
        //return renderer
        renderer_ref *getRenderer( void );
        //return model count
        unsigned int getModelCount( void );
        //get camera position
        void getCameraPosition( dpposition *p );
        //set camera position
        void setCameraPosition( dpposition *p );
        //get models
        bool getModels( model_man_ref **r );
        //get models
        bool getModels( model_man_readlock **r, shared_obj_guard *o );
        //get models
        bool getModels( model_man_writelock **r, shared_obj_guard *o );
        //get guis
        bool getGuis( gui_man_ref **r );
        //get guis
        bool getGuis( gui_man_readlock **r, shared_obj_guard *o );
        //get guis
        bool getGuis( gui_man_writelock **r, shared_obj_guard *o );
        //get actors
        bool getActors( dpactor_man_ref **r );
        //get actors
        bool getActors( dpactor_man_readlock **r, shared_obj_guard *o );
        //get actors
        bool getActors( dpactor_man_writelock **r, shared_obj_guard *o );
        //add renderer factory
        void addRenderer( renderer_factory *f );
        //change renderer
        bool changeRenderer( const char *cname );
        
    public:

        //ctor
        gfx( core *c, dptaskpool_writelock *tp );
        //dtor
        virtual ~gfx( void );
        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );

        friend class gfx_readlock;
        friend class gfx_writelock;
        friend class gfx_task;
    };
    
};

#endif