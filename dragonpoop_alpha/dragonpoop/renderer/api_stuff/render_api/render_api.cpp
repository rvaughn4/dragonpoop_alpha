
#include "render_api.h"
#include "render_api_ref.h"
#include "render_api_readlock.h"
#include "render_api_writelock.h"

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
    
    //generate read lock
    shared_obj_readlock *render_api::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_readlock( (render_api *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_writelock( (render_api *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_ref( (render_api *)p, k );
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
