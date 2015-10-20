
#include "render_api.h"

namespace dragonpoop
{
    
    //ctor
    render_api::render_api( window *w, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->w = w;
    }
    
    //dtor
    render_api::~render_api( void )
    {
        delete this->w;
    }
    
    //run api
    void render_api::run( void )
    {
        this->w->run();
    }
    
    //returns true if window is open
    bool render_api::isOpen( void )
    {
        return this->w->isOpen();
    }
    
    //returns window width
    float render_api::getWidth( void )
    {
        return this->w->getWidth();
    }
    
    //returns window height
    float render_api::getHeight( void )
    {
        return this->w->getHeight();
    }

    //returns window
    window *render_api::getWindow( void )
    {
        return this->w;
    }
    
};
