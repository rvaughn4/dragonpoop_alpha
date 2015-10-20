
#include "render_api_shader.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_shader_ref.h"
#include "render_api_shader_readlock.h"
#include "render_api_shader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader::render_api_shader( render_api_writelock *r, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->r = (render_api_ref *)r->getRef();
    }
    
    //dtor
    render_api_shader::~render_api_shader( void )
    {
        delete this->r;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_shader::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_shader_readlock( (render_api_shader *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_shader::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_shader_writelock( (render_api_shader *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_shader::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_shader_ref( (render_api_shader *)p, k );
    }
    
};
