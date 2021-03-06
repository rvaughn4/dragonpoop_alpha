
#include "renderer_commandlist_passer_writelock.h"
#include "renderer_commandlist_passer.h"

namespace dragonpoop
{

    //ctor
    renderer_commandlist_passer_writelock::renderer_commandlist_passer_writelock( renderer_commandlist_passer *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_commandlist_passer_writelock::~renderer_commandlist_passer_writelock( void )
    {
        
    }
    
    //set camera position
    void renderer_commandlist_passer_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }
    
    //get camera position
    dpposition *renderer_commandlist_passer_writelock::getPosition( void )
    {
        return this->t->getPosition();
    }
    
    //set model commandlist
    void renderer_commandlist_passer_writelock::setModel( render_api_commandlist_ref *r )
    {
        this->t->setModel( r );
    }
    
    //get model commandlist
    render_api_commandlist_ref *renderer_commandlist_passer_writelock::getModel( void )
    {
        return this->t->getModel();
    }
    
    //set gui commandlist
    void renderer_commandlist_passer_writelock::setGui( render_api_commandlist_ref *r )
    {
        this->t->setGui( r );
    }
    
    //get gui commandlist
    render_api_commandlist_ref *renderer_commandlist_passer_writelock::getGui( void )
    {
        return this->t->getGui();
    }
    
    //set land commandlist
    void renderer_commandlist_passer_writelock::setLand( render_api_commandlist_ref *r )
    {
        this->t->setLand( r );
    }
    
    //get land commandlist
    render_api_commandlist_ref *renderer_commandlist_passer_writelock::getLand( void )
    {
        return this->t->getLand();
    }
    
    //set sky commandlist
    void renderer_commandlist_passer_writelock::setSky( render_api_commandlist_ref *r )
    {
        this->t->setSky( r );
    }
    
    //get sky commandlist
    render_api_commandlist_ref *renderer_commandlist_passer_writelock::getSky( void )
    {
        return this->t->getSky();
    }
    
};
