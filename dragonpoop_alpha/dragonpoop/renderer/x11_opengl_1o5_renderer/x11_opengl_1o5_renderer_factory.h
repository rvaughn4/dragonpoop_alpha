
#ifndef dragonpoop_x11_opengl_1o5_renderer_factory_h
#define dragonpoop_x11_opengl_1o5_renderer_factory_h

#include "../renderer_factory.h"

namespace dragonpoop
{
    
    class x11_opengl_1o5_renderer_factory : public renderer_factory
    {
        
    private:
        
        bool bUseDl;
        
    protected:
        
        //generate renderer
        virtual renderer *genRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //test renderer
        virtual bool testRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        
    public:
        
        //ctor
        x11_opengl_1o5_renderer_factory( unsigned int score, bool bUseDl );
        //dtor
        virtual ~x11_opengl_1o5_renderer_factory( void );
        
    };
    
};

#endif