
#include "gfx_writelock.h"
#include "gfx.h"

namespace dragonpoop
{

    //ctor
    gfx_writelock::gfx_writelock( gfx *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    gfx_writelock::~gfx_writelock( void )
    {

    }

    //returns true if running
    bool gfx_writelock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *gfx_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //stop gfx task
    void gfx_writelock::kill( void )
    {
        this->t->kill();
    }

    //run gfx from task
    void gfx_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
    
    //create model using name (if not exists, reuses if does), returns ref in pointer arg
    bool gfx_writelock::createModel( dpthread_lock *thd, const char *mname, model_ref **r )
    {
        return this->t->createModel( thd, mname, r );
    }
    
    //create model and load model file into it
    bool gfx_writelock::loadModel( dpthread_lock *thd, const char *mname, const char *file_name, model_ref **r, model_loader **mldr )
    {
        return this->t->loadModel( thd, mname, file_name, r, mldr );
    }
    
    //find model by name
    model_ref *gfx_writelock::findModel( const char *cname )
    {
        return this->t->findModel( cname );
    }
    
    //find model by id
    model_ref *gfx_writelock::findModel( dpid id )
    {
        return this->t->findModel( id );
    }
    
    //get a model instance by name
    model_instance_ref *gfx_writelock::makeModelInstance( const char *cname )
    {
        return this->t->makeModelInstance( cname );
    }
    
    //get a model instance by id
    model_instance_ref *gfx_writelock::makeModelInstance( dpid id )
    {
        return this->t->makeModelInstance( id );
    }
    
};
