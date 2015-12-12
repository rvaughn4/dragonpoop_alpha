
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

    //get camera position
    dpposition_share_ref *gfx_readlock::getCameraPosition( void )
    {
        return this->t->getCameraPosition();
    }

    //get height cache
    dpheight_cache_ref *gfx_readlock::getHeights( void )
    {
        return this->t->getHeights();
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

    //get guis
    bool gfx_readlock::getGuis( gui_man_ref **r )
    {
        return this->t->getGuis( r );
    }

    //get guis
    bool gfx_readlock::getGuis( gui_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getGuis( r, o );
    }

    //get guis
    bool gfx_readlock::getGuis( gui_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getGuis( r, o );
    }

    //get land
    bool gfx_readlock::getLand( dpland_man_ref **r )
    {
        return this->t->getLand( r );
    }

    //get land
    bool gfx_readlock::getLand( dpland_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getLand( r, o );
    }

    //get land
    bool gfx_readlock::getLand( dpland_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getLand( r, o );
    }

    //get sky
    bool gfx_readlock::getSky( dpsky_man_ref **r )
    {
        return this->t->getSky( r );
    }

    //get sky
    bool gfx_readlock::getSky( dpsky_man_readlock **r, shared_obj_guard *o )
    {
        return this->t->getSky( r, o );
    }

    //get sky
    bool gfx_readlock::getSky( dpsky_man_writelock **r, shared_obj_guard *o )
    {
        return this->t->getSky( r, o );
    }

    //returns taskpool
    dptaskpool_ref *gfx_readlock::getTasks( void )
    {
        return this->t->getTasks();
    }

};
