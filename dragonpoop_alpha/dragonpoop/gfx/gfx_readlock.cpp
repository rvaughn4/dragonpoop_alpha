
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
    
    //find model by name
    model_ref *gfx_readlock::findModel( const char *cname )
    {
        return this->t->findModel( cname );
    }
    
    //find model by id
    model_ref *gfx_readlock::findModel( dpid id )
    {
        return this->t->findModel( id );
    }
    
    //get models
    void gfx_readlock::getModels( std::list<model *> *l )
    {
        this->t->getModels( l );
    }

    //get guis
    void gfx_readlock::getGuis( std::list<gui_ref *> *l )
    {
        this->t->getGuis( l );
    }
    
};
