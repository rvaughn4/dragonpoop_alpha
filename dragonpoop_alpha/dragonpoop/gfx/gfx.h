
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

    class gfx : public shared_obj
    {

    private:

        struct
        {
            struct
            {
                dptask *tsk;
                gfx_task *gtsk;
            } tgfx, tmodels, tguis, tactors;
        } tasks;
        
        dpland_man *land_mgr;
        dpactor_man *actor_mgr;
        model_man *model_mgr;
        
        core *c;
        renderer *r;
        dptaskpool_ref *tpr;
        unsigned int ms_each_frame, model_cnt, gui_cnt;
        float fps;
        uint64_t last_r_poll;
        dpposition cam_pos;
        bool bIsRun;
        
        gui_factory_ref *root_factory;
        gui *root_g;
        
        std::list<gui_ref *> guis;
        
        //start all tasks
        void _startTasks( core *c, dptaskpool_writelock *tp );
        //start task
        void _startTask( core *c, dptaskpool_writelock *tp, gfx_task **pgtsk, dptask **ptsk, bool bRunGfx, bool bRunModels, bool bRunGuis, bool bRunActors, unsigned int ms_delay );
        //kill all tasks
        void _killTasks( void );
        //kill task
        void _killTask( gfx_task **pgtsk, dptask **ptsk );
        //delete all tasks
        void _deleteTasks( void );
        //delete task
        void _deleteTask( gfx_task **pgtsk, dptask **ptsk );
        
        //delete all guis
        void deleteGuis( void );
        //run all guis
        static void runGuis( dpthread_lock *thd, gfx_ref *g );
        
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
        //add gui
        void addGui( gui *g );
        //add gui
        void addGui( gui_ref *g );
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //return fps
        float getFps( void );
        //return ms each frame
        unsigned int getMsEachFrame( void );
        //set root gui factory
        void setRootGui( gui_factory *g );
        //return renderer
        renderer_ref *getRenderer( void );
        //return model count
        unsigned int getModelCount( void );
        //return gui count
        unsigned int getGuiCount( void );
        //get camera position
        void getCameraPosition( dpposition *p );
        //set camera position
        void setCameraPosition( dpposition *p );
        //add actor
        void addActor( dpactor *a );
        //add actor
        void addActor( dpactor_ref *a );
        //return actor count
        unsigned int getActorCount( void );
        
        //get models
        bool getModels( model_man_ref **r );
        //get models
        bool getModels( model_man_readlock **r, shared_obj_guard *o );
        //get models
        bool getModels( model_man_writelock **r, shared_obj_guard *o );

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