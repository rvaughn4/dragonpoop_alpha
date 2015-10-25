
#include "opengl1o5_x11_indexbuffer.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "opengl1o5_x11_indexbuffer_readlock.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_indexbuffer::opengl1o5_x11_indexbuffer( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpindex_buffer *vb ) : render_api_indexbuffer( r, c, mm, vb )
    {
        this->vb.copy( vb );
    }
    
    //dtor
    opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer" );
        this->vb.clear();
        o.unlock();
    }
    
    //generate read lock
    shared_obj_readlock *opengl1o5_x11_indexbuffer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new opengl1o5_x11_indexbuffer_readlock( (opengl1o5_x11_indexbuffer *)p, l );
    }
    
    //return vb
    dpindex_buffer *opengl1o5_x11_indexbuffer::getBuffer( void )
    {
        return &this->vb;
    }
    
};
