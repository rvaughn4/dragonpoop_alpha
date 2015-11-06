
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
  
    //return renderer
    renderer_ref *gfx_writelock::getRenderer( void )
    {
        return this->t->getRenderer();
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
    
    //get guis
    bool gfx_writelock::getGuis( gui_man_ref **r )
    {
        return this->t->getGuis( r );
    }
    
    //get guis
    bool gfx_writelock::getGuis( gui_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getGuis( r, o );
    }
    
    //get guis
    bool gfx_writelock::getGuis( gui_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getGuis( r, o );
    }

    //get actors
    bool gfx_writelock::getActors( dpactor_man_ref **r )
    {
        return this->t->getActors( r );
    }
    
    //get actors
    bool gfx_writelock::getActors( dpactor_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getActors( r, o );
    }
    
    //get actors
    bool gfx_writelock::getActors( dpactor_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getActors( r, o );
    }
    
    //get land
    bool gfx_writelock::getLand( dpland_man_ref **r )
    {
        return this->t->getLand( r );
    }
    
    //get land
    bool gfx_writelock::getLand( dpland_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getLand( r, o );
    }
    
    //get land
    bool gfx_writelock::getLand( dpland_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getLand( r, o );
    }
    
    //add renderer factory
    void gfx_writelock::addRenderer( renderer_factory *f )
    {
        this->t->addRenderer( f );
    }
    
    //change renderer
    bool gfx_writelock::changeRenderer( const char *cname )
    {
        return this->t->changeRenderer( cname );
    }
    
};
