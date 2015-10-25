
#include "opengl1o5_x11_vertexbuffer.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_vertexbuffer::opengl1o5_x11_vertexbuffer( render_api_writelock *r, render_api_context_writelock *c, dpmutex_master *mm, dpvertex_buffer *vb ) : render_api_vertexbuffer( r, c, mm, vb )
    {
        this->vb.copy( vb );
    }
    
    //dtor
    opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer" );
        this->vb.clear();
        o.unlock();
    }
    
};
