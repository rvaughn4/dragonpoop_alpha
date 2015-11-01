
#include "render_api_indexbuffer.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_indexbuffer_ref.h"
#include "render_api_indexbuffer_readlock.h"
#include "render_api_indexbuffer_writelock.h"

namespace dragonpoop
{
    
    //ctor
    render_api_indexbuffer::render_api_indexbuffer( render_api_context_writelock *c, dpmutex_master *mm, dpindex_buffer *ib ) : shared_obj( mm )
    {
        this->c = (render_api_context_ref *)c->getRef();
    }
    
    //dtor
    render_api_indexbuffer::~render_api_indexbuffer( void )
    {
        delete this->c;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_indexbuffer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_indexbuffer_readlock( (render_api_indexbuffer *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_indexbuffer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_indexbuffer_writelock( (render_api_indexbuffer *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_indexbuffer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_indexbuffer_ref( (render_api_indexbuffer *)p, k );
    }
    
};
