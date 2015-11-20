
#ifndef dragonpoop_model_man_h
#define dragonpoop_model_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{

    class dpthread_lock;
    class core;
    class model_man_ref;
    class model_man_readlock;
    class model_man_writelock;
    class gfx_ref;
    class dptask;
    class model_man_task;
    class dptaskpool_writelock;
    class gfx;
    class model;
    class model_ref;
    class model_loader_man;
    class model_loader_ref;
    class model_saver_ref;
    class model_instance_ref;
    class dptaskpool_ref;

    class model_man : public shared_obj
    {

    private:

        core *c;
        gfx_ref *g;
        dptask *tsk;
        model_man_task *gtsk;
        std::list<model *> models;
        int model_cnt, instance_cnt;
        model_loader_man *loader_mgr;
        dptaskpool_ref *tpr;
        unsigned int ms_each_frame;

        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //delete all models
        void deleteModels( void );
        //run all models
        void runModels( dpthread_lock *thd );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, model_man_writelock *g );
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
        //get models
        void getModels( std::list<model *> *l );
        //return count of loaded models
        unsigned int getModelCount( void );
        //return model instance count
        unsigned int getInstanceCount( void );

    public:

        //ctor
        model_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~model_man( void );
        //return core
        core *getCore( void );

        friend class model_man_readlock;
        friend class model_man_writelock;

    };

};

#endif
