
#include "model_saver_writelock.h"
#include "model_saver.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_writelock::model_saver_writelock( model_saver *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_saver_writelock::~model_saver_writelock( void )
    {
        
    }
    
    //returns true if running
    bool model_saver_writelock::isRunning( void )
    {
        return this->t->isRunning();
    }
    
    //returns true if sucessful
    bool model_saver_writelock::wasSucessful( void )
    {
        return this->t->wasSucessful();
    }
    
    //run model saver from task
    void model_saver_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //get model saver
    model_saver *model_saver_writelock::getSaver( void )
    {
        return this->t;
    }
    
    //returns model
    model_ref *model_saver_writelock::getModel( void )
    {
        return this->t->getModel();
    }
    
};
