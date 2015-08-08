
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
    class model_loader;
    class model_instance_ref;

    class gfx : public shared_obj
    {

    private:

        dptask *tsk;
        gfx_task *gtsk;
        core *c;
        renderer *r;
        dptaskpool_ref *tpr;
        
        std::list<model *> models;
        std::list<model_loader *> loaders;
        
        //delete all models
        void deleteModels( void );
        //delete all loaders
        void deleteLoaders( void );
        //delete old models
        void runModels( void );
        //delete old loaders
        void runLoaders( void );

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
        bool loadModel( const char *mname, const char *file_name, model_ref **r, model_loader **mldr );
        //find model by name
        model_ref *findModel( const char *cname );
        //find model by id
        model_ref *findModel( dpid id );
        //get a model instance by name
        model_instance_ref *makeModelInstance( const char *cname );
        //get a model instance by id
        model_instance_ref *makeModelInstance( dpid id );
        //get models
        void getModels( std::list<model_ref *> *l );

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
    };
    
};

#endif