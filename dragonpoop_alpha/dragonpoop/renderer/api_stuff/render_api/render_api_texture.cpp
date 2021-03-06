
#include "render_api_texture.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_texture_ref.h"
#include "render_api_texture_readlock.h"
#include "render_api_texture_writelock.h"

namespace dragonpoop
{

    //ctor
    render_api_texture::render_api_texture( render_api_context_writelock *c, dpmutex_master *mm, dpbitmap *bm ) : shared_obj( mm )
    {
        this->c = (render_api_context_ref *)c->getRef();
    }

    //dtor
    render_api_texture::~render_api_texture( void )
    {
        delete this->c;
    }

    //generate read lock
    shared_obj_readlock *render_api_texture::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_texture_readlock( (render_api_texture *)p, l );
    }

    //generate write lock
    shared_obj_writelock *render_api_texture::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_texture_writelock( (render_api_texture *)p, l );
    }

    //generate ref
    shared_obj_ref *render_api_texture::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_texture_ref( (render_api_texture *)p, k );
    }

    //returns true if works
    bool render_api_texture::works( void )
    {
        return 0;
    }

};
