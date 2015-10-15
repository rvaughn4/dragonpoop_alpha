
#include "gfx_readlock.h"
#include "gfx.h"

namespace dragonpoop
{

    //ctor
    gfx_readlock::gfx_readlock( gfx *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    gfx_readlock::~gfx_readlock( void )
    {

    }

    //returns true if running
    bool gfx_readlock::isRunning( void )
    {
        return this->t->isRunning();
    }

    //return core
    core *gfx_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get guis
    void gfx_readlock::getGuis( std::list<gui_ref *> *l )
    {
        this->t->getGuis( l );
    }
    
    //return fps
    float gfx_readlock::getFps( void )
    {
        return this->t->getFps();
    }
    
    //return ms each frame
    unsigned int gfx_readlock::getMsEachFrame( void )
    {
        return this->t->getMsEachFrame();
    }
    
    //return renderer
    renderer_ref *gfx_readlock::getRenderer( void )
    {
        return this->t->getRenderer();
    }
    
    //return model count
    unsigned int gfx_readlock::getModelCount( void )
    {
        return this->t->getModelCount();
    }
    
    //return gui count
    unsigned int gfx_readlock::getGuiCount( void )
    {
        return this->t->getGuiCount();
    }
    
    //get camera position
    void gfx_readlock::getCameraPosition( dpposition *p )
    {
        this->t->getCameraPosition( p );
    }

    //return actor count
    unsigned int gfx_readlock::getActorCount( void )
    {
        return this->t->getActorCount();
    }
  
    //get models
    bool gfx_readlock::getModels( model_man_ref **r )
    {
        return this->t->getModels( r );
    }
    
    //get models
    bool gfx_readlock::getModels( model_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getModels( r, o );
    }
    
    //get models
    bool gfx_readlock::getModels( model_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getModels( r, o );
    }
    
};
