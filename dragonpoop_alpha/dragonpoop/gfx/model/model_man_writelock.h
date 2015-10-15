
#ifndef dragonpoop_model_man_writelock_h
#define dragonpoop_model_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class model_man;
    class core;
    class dpthread_lock;
    class model_ref;
    class model;
    class model_saver_ref;
    class model_loader_ref;
    
    class model_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_man *t;
        
    protected:
        
        //ctor
        model_man_writelock( model_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
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
        //get models
        void getModels( std::list<model *> *l );

        friend class model_man;
    };
    
};

#endif