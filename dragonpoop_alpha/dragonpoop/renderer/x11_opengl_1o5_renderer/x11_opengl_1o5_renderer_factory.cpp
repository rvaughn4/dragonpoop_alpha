
#include "x11_opengl_1o5_renderer_factory.h"
#include "x11_opengl_1o5_renderer.h"
#include "../api_stuff/opengl1o5_x11/opengl1o5_x11.h"
#include "../api_stuff/render_api/render_api_writelock.h"
#include "../../core/core.h"
#include "../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    x11_opengl_1o5_renderer_factory::x11_opengl_1o5_renderer_factory( unsigned int score, bool bUseDl ) : renderer_factory( bUseDl ? "X11 OpenGL 1.5 with Display Lists" : "X11 OpenGL 1.5", score )
    {
        this->bUseDl = bUseDl;
    }
    
    //dtor
    x11_opengl_1o5_renderer_factory::~x11_opengl_1o5_renderer_factory( void )
    {
        
    }
    
    //generate renderer
    renderer *x11_opengl_1o5_renderer_factory::genRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        return new x11_opengl_1o5_renderer( c, g, tp, this->bUseDl );
    }
    
    //test renderer
    bool x11_opengl_1o5_renderer_factory::testRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        opengl1o5_x11 *tr;
        render_api_writelock *l;
        shared_obj_guard o;
        bool r;
        
        tr = new opengl1o5_x11( 20, 20, "testing x11 opengl 1.5 ...", c->getMutexMaster(), this->bUseDl );
        if( !tr )
            return 0;
        
        r = 0;
        l = (render_api_writelock *)o.tryWriteLock( tr, 3000, "x11_opengl_1o5_renderer_factory::testRenderer" );
        if( l )
            r = l->isOpen();
        o.unlock();
        
        delete tr;
        
        return r;
    }
    
};
