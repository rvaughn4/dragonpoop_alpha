
#include "model_man_writelock.h"
#include "model_man.h"

namespace dragonpoop
{
    
    //ctor
    model_man_writelock::model_man_writelock( model_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_man_writelock::~model_man_writelock( void )
    {
        
    }
    
    //return core
    core *model_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void model_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //create model using name (if not exists, reuses if does), returns ref in pointer arg
    bool model_man_writelock::createModel( const char *mname, model_ref **r )
    {
        return this->t->createModel( mname, r );
    }
    
    //create model and load model file into it
    bool model_man_writelock::loadModel( const char *mname, const char *path_name, const char *file_name, model_ref **r, model_loader_ref **mldr )
    {
        return this->t->loadModel( mname, path_name, file_name, r, mldr );
    }
    
    //find model and save model file
    bool model_man_writelock::saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr )
    {
        return this->t->saveModel( mname, path_name, file_name, msvr );
    }
    
    //find model by name
    model_ref *model_man_writelock::findModel( const char *cname )
    {
        return this->t->findModel( cname );
    }
    
    //find model by id
    model_ref *model_man_writelock::findModel( dpid id )
    {
        return this->t->findModel( id );
    }
    
    //get models
    void model_man_writelock::getModels( std::list<model *> *l )
    {
        this->t->getModels( l );
    }
    
};
