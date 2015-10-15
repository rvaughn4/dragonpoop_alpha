
#include "model_man_readlock.h"
#include "model_man.h"

namespace dragonpoop
{
    
    //ctor
    model_man_readlock::model_man_readlock( model_man *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_man_readlock::~model_man_readlock( void )
    {
        
    }
    
    //return core
    core *model_man_readlock::getCore( void )
    {
        return this->t->getCore();
    }
  
    //find model and save model file
    bool model_man_readlock::saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr )
    {
        return this->t->saveModel( mname, path_name, file_name, msvr );
    }
    
    //find model by name
    model_ref *model_man_readlock::findModel( const char *cname )
    {
        return this->t->findModel( cname );
    }
    
    //find model by id
    model_ref *model_man_readlock::findModel( dpid id )
    {
        return this->t->findModel( id );
    }
    
    //get models
    void model_man_readlock::getModels( std::list<model *> *l )
    {
        this->t->getModels( l );
    }

    //return count of loaded models
    unsigned int model_man_readlock::getModelCount( void )
    {
        return this->t->getModelCount();
    }
    
    //return model instance count
    unsigned int model_man_readlock::getInstanceCount( void )
    {
        return this->t->getInstanceCount();
    }
    
};
