
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
  
    //add gui
    void gfx_writelock::addGui( gui *g )
    {
        this->t->addGui( g );
    }
    
    //add gui
    void gfx_writelock::addGui( gui_ref *g )
    {
        this->t->addGui( g );
    }
    
    //get guis
    void gfx_writelock::getGuis( std::list<gui_ref *> *l )
    {
        this->t->getGuis( l );
    }
  
    //set root gui factory
    void gfx_writelock::setRootGui( gui_factory *g )
    {
        this->t->setRootGui( g );
    }
    
    //return renderer
    renderer_ref *gfx_writelock::getRenderer( void )
    {
        return this->t->getRenderer();
    }
    
    //return model count
    unsigned int gfx_writelock::getModelCount( void )
    {
        return this->t->getModelCount();
    }
    
    //return gui count
    unsigned int gfx_writelock::getGuiCount( void )
    {
        return this->t->getGuiCount();
    }
    
    //get camera position
    void gfx_writelock::getCameraPosition( dpposition *p )
    {
        this->t->getCameraPosition( p );
    }
    
    //set camera position
    void gfx_writelock::setCameraPosition( dpposition *p )
    {
        this->t->setCameraPosition( p );
    }
    
    //add actor
    void gfx_writelock::addActor( dpactor *a )
    {
        this->t->addActor( a );
    }
    
    //add actor
    void gfx_writelock::addActor( dpactor_ref *a )
    {
        this->t->addActor( a );
    }
    
    //return actor count
    unsigned int gfx_writelock::getActorCount( void )
    {
        return this->t->getActorCount();
    }
    
    //get models
    bool gfx_writelock::getModels( model_man_ref **r )
    {
        return this->t->getModels( r );
    }
    
    //get models
    bool gfx_writelock::getModels( model_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getModels( r, o );
    }
    
    //get models
    bool gfx_writelock::getModels( model_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getModels( r, o );
    }
    
};
