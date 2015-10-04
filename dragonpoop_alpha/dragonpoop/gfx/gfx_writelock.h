
#ifndef dragonpoop_gfx_writelock_h
#define dragonpoop_gfx_writelock_h

#include "../core/shared_obj/shared_obj_writelock.h"
#include "../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class gfx;
    class core;
    class dpthread_lock;
    class model_ref;
    class model;
    class model_loader_ref;
    class model_instance_ref;
    class model_saver_ref;
    class gui;
    class gui_ref;
    class gui_factory;

    class gfx_writelock : public shared_obj_writelock
    {

    private:

        gfx *t;

    protected:

        //ctor
        gfx_writelock( gfx *t, dpmutex_writelock *l );
        //dtor
        virtual ~gfx_writelock( void );

    public:

        //returns true if running
        bool isRunning( void );
        //return core
        core *getCore( void );
        //stop gfx task
        void kill( void );
        //run gfx from task
        void run( dpthread_lock *thd );
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
        //get a model instance by name
        dpid makeModelInstance( const char *cname, model_instance_ref **r );
        //get a model instance by id
        dpid makeModelInstance( dpid id, model_instance_ref **r );
        //get models
        void getModels( std::list<model *> *l );
        //start animation by name (returns animation instance id)
        dpid startAnimation( const char *mname, dpid minstance_id, const char *anim_name, bool do_repeat, float speed );
        //stop all animations
        void stopAllAnimations( const char *mname, dpid minstance_id );
        //add gui
        void addGui( gui *g );
        //add gui
        void addGui( gui_ref *g );
        //get guis
        void getGuis( std::list<gui_ref *> *l );
        //set root gui factory
        void setRootGui( gui_factory *g );

        friend class gfx;
    };
    
};

#endif