
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
    bool gfx_writelock::createModel( const char *mname, model_ref **r )
    {
        return this->t->createModel( mname, r );
    }
    
    //create model and load model file into it
    bool gfx_writelock::loadModel( const char *mname, const char *path_name, const char *file_name, model_ref **r, model_loader_ref **mldr )
    {
        return this->t->loadModel( mname, path_name, file_name, r, mldr );
    }
    
    //find model and save model file
    bool gfx_writelock::saveModel( const char *mname, const char *path_name, const char *file_name, model_saver_ref **msvr )
    {
        return this->t->saveModel( mname, path_name, file_name, msvr );
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
    dpid gfx_writelock::makeModelInstance( const char *cname, model_instance_ref **r )
    {
        return this->t->makeModelInstance( cname, r );
    }
    
    //get a model instance by id
    dpid gfx_writelock::makeModelInstance( dpid id, model_instance_ref **r )
    {
        return this->t->makeModelInstance( id, r );
    }
    
    //get models
    void gfx_writelock::getModels( std::list<model_ref *> *l )
    {
        this->t->getModels( l );
    }
    
    //start animation by name (returns animation instance id)
    dpid gfx_writelock::startAnimation( const char *mname, dpid minstance_id, const char *anim_name, bool do_repeat, float speed )
    {
        return this->t->startAnimation( mname, minstance_id, anim_name, do_repeat, speed );
    }
    
};
