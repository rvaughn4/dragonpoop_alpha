
#include "x11_opengl_1o5_renderer.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../api_stuff/opengl1o5_x11/opengl1o5_x11.h"
#include <string>

namespace dragonpoop
{

    //ctor
    x11_opengl_1o5_renderer::x11_opengl_1o5_renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp, bool bUseDl ) : renderer( c, g, tp )
    {
        this->bUseDl = bUseDl;
    }

    //dtor
    x11_opengl_1o5_renderer::~x11_opengl_1o5_renderer( void )
    {
        shared_obj_guard o;

        this->_kill();

        o.tryWriteLock( this, 3000, "x11_opengl_1o5_renderer::~x11_opengl_1o5_renderer" );
        o.unlock();
        this->unlink();
    }

    //get renderer name
    void x11_opengl_1o5_renderer::getName( std::string *s )
    {
        if( this->bUseDl )
            s->assign( "X11 OpenGL 1.5 with Display Lists" );
        else
            s->assign( "X11 OpenGL 1.5" );
    }

    //generate render api
    render_api *x11_opengl_1o5_renderer::genRenderApi( dpmutex_master *mm, input_passer_writelock *ip )
    {
        return new opengl1o5_x11( 640, 480, "hello", mm, this->bUseDl, ip );
    }

};
