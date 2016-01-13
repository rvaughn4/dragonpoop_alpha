
#include "render_api_writelock.h"
#include "render_api.h"

namespace dragonpoop
{

    //ctor
    render_api_writelock::render_api_writelock( render_api *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    render_api_writelock::~render_api_writelock( void )
    {

    }

    //run api
    void render_api_writelock::run( void )
    {
        this->t->run();
    }

    //returns true if window is open
    bool render_api_writelock::isOpen( void )
    {
        return this->t->isOpen();
    }

    //returns window width
    float render_api_writelock::getWidth( void )
    {
        return this->t->getWidth();
    }

    //returns window height
    float render_api_writelock::getHeight( void )
    {
        return this->t->getHeight();
    }

    //make context
    render_api_context_ref *render_api_writelock::getContext( void )
    {
        return this->t->getContext( this );
    }

    //delete contexts
    void render_api_writelock::deleteContexts( void )
    {
        this->t->deleteContexts();
    }

};
