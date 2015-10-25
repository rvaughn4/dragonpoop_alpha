
#include "render_api_shader.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_shader_ref.h"
#include "render_api_shader_readlock.h"
#include "render_api_shader_writelock.h"

namespace dragonpoop
{
    
    //ctor
    render_api_shader::render_api_shader( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->r = (render_api_ref *)r->getRef();
        this->c = (render_api_context_ref *)c->getRef();
    }
    
    //dtor
    render_api_shader::~render_api_shader( void )
    {
        delete this->c;
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
    
    //render vb
    void render_api_shader::render( render_api_context_writelock *ctx, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_indexbuffer_ref *ib, render_api_vertexbuffer_ref *vb, dpmatrix *m, float alpha )
    {
        
    }
    
};
