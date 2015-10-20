
#include "render_api_context.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_readlock.h"
#include "render_api_context_writelock.h"

namespace dragonpoop
{

    //ctor
    render_api_context::render_api_context( render_api_writelock *r, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->r = (render_api_ref *)r->getRef();
    }
    
    //dtor
    render_api_context::~render_api_context( void )
    {
        delete this->r;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_context::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_context_readlock( (render_api_context *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_context::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_context_writelock( (render_api_context *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_context::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_context_ref( (render_api_context *)p, k );
    }

};
