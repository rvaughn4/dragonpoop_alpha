
#include "opengl1o5_x11_indexbuffer.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_indexbuffer::opengl1o5_x11_indexbuffer( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpindex_buffer *ib ) : render_api_indexbuffer( r, c, mm, ib )
    {
        this->ib.copy( ib );
    }
    
    //dtor
    opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_indexbuffer::~opengl1o5_x11_indexbuffer" );
        this->ib.clear();
        o.unlock();
    }
    
};
