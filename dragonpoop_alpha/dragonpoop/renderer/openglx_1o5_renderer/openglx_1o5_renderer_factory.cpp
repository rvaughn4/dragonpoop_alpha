
#include "openglx_1o5_renderer_factory.h"
#include "openglx_1o5_renderer.h"

namespace dragonpoop
{
    
    //ctor
    openglx_1o5_renderer_factory::openglx_1o5_renderer_factory( void ) : renderer_factory( "X11 OpenGL 1.5", 1 )
    {
        
    }
    
    //dtor
    openglx_1o5_renderer_factory::~openglx_1o5_renderer_factory( void )
    {
        
    }
    
    //generate renderer
    renderer *openglx_1o5_renderer_factory::genRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        return new openglx_1o5_renderer( c, g, tp );
    }
    
    //test renderer
    bool openglx_1o5_renderer_factory::testRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp )
    {
        return 1;
    }
    
};
