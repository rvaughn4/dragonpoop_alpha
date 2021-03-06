
#include "render_api_vertexbuffer.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_vertexbuffer_ref.h"
#include "render_api_vertexbuffer_readlock.h"
#include "render_api_vertexbuffer_writelock.h"

namespace dragonpoop
{
    
    //ctor
    render_api_vertexbuffer::render_api_vertexbuffer( render_api_context_writelock *c, dpmutex_master *mm, dpvertex_buffer *vb ) : shared_obj( mm )
    {
        this->c = (render_api_context_ref *)c->getRef();
    }
    
    //dtor
    render_api_vertexbuffer::~render_api_vertexbuffer( void )
    {
        delete this->c;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_vertexbuffer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_vertexbuffer_readlock( (render_api_vertexbuffer *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_vertexbuffer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_vertexbuffer_writelock( (render_api_vertexbuffer *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_vertexbuffer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_vertexbuffer_ref( (render_api_vertexbuffer *)p, k );
    }
    
};
