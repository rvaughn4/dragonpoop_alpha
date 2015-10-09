
#ifndef dragonpoop_gfx_h
#define dragonpoop_gfx_h

#include "../core/shared_obj/shared_obj.h"
#include "../core/bytetree/dpid_bytetree.h"

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

    class gfx : public shared_obj
    {

    private:

        dptask *tsk;
        gfx_task *gtsk;
        core *c;
        renderer *r;
        dptaskpool_ref *tpr;
        unsigned int ms_each_frame;
        float fps;
        uint64_t last_r_poll;
        
        gui_factory_ref *root_factory;
        gui *root_g;
        
        std::list<model *> models;
        std::list<model_loader *> loaders;
        std::list<model_saver *> savers;
        std::list<gui_ref *> guis;
        
        //delete all models
        void deleteModels( void );
        //delete all loaders
        void deleteLoaders( void );
        //delete all savers
        void deleteSavers( void );
        //delete all guis
        void deleteGuis( void );
        //delete old models
        static void runModels( dpthread_lock *thd, gfx_ref *g );
        //delete old loaders
        void runLoaders( dpthread_lock *thd );
        //delete old savers
        void runSavers( dpthread_lock *thd );
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
        //create model using name (if not exists, reuses if does), returns ref in pointer arg
        bool createModel( const char *mname, model_ref **r );
        //create model and load model file into it
        bool loadModel( const char *mname, const char *path_name, const char *file_name, model_ref **r, model_loader_ref **mldr );
        //find model and save model file
        bool saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr );
        //find model by name
        model_ref *findModel( const char *cname );
        //find model by id
        model_ref *findModel( dpid id );
        //get a model instance by name (returns instance id)
        dpid makeModelInstance( const char *cname, model_instance_ref **r );
        //get a model instance by id (returns instance id)
        dpid makeModelInstance( dpid id, model_instance_ref **r );
        //get a model instance (returns instance id)
        dpid makeModelInstance( model_ref *m, model_instance_ref **r );
        //get models
        void getModels( std::list<model *> *l );
        //start animation by name (returns animation instance id)
        dpid startAnimation( const char *mname, dpid minstance_id, const char *anim_name, bool do_repeat, float speed );
        //stop animation by name
        void stopAnimation( const char *mname, dpid minstance_id, const char *anim_name );
        //stop all animations
        void stopAllAnimations( const char *mname, dpid minstance_id );
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