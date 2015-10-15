
#ifndef dragonpoop_model_man_readlock_h
#define dragonpoop_model_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class model_man;
    class core;
    class model_ref;
    class model;
    class model_saver_ref;
    class model_loader_ref;

    class model_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_man *t;
        
    protected:
        
        //ctor
        model_man_readlock( model_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
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

        friend class model_man;
    };
    
};

#endif