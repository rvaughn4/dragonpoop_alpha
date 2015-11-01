
#ifndef dragonpoop_x11_opengl_1o5_renderer_h
#define dragonpoop_x11_opengl_1o5_renderer_h

#include "../renderer.h"

namespace dragonpoop
{
    
    class x11_opengl_1o5_renderer : public renderer
    {
        
    private:
        
        bool bUseDl;
        
    protected:
        
        //get renderer name
        virtual void getName( std::string *s );
        //generate render api
        virtual render_api *genRenderApi( dpmutex_master *mm );
        
    public:
        
        //ctor
        x11_opengl_1o5_renderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp, bool bUseDl );
        //dtor
        virtual ~x11_opengl_1o5_renderer( void );
        
    };
    
};

#endif