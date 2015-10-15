
#include "model_loader_man_writelock.h"
#include "model_loader_man.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_man_writelock::model_loader_man_writelock( model_loader_man *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_loader_man_writelock::~model_loader_man_writelock( void )
    {
        
    }
    
    //return core
    core *model_loader_man_writelock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //run
    void model_loader_man_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
  
    //load file into model
    bool model_loader_man_writelock::load( model_ref *m, const char *path_name, const char *file_name, model_loader_ref **mldr )
    {
        return this->t->load( m, path_name, file_name, mldr );
    }
    
    //save model into file
    bool model_loader_man_writelock::save( model_ref *m, const char *path_name, const char *file_name, model_saver_ref **mldr )
    {
        return this->t->save( m, path_name, file_name, mldr );
    }
    
};
