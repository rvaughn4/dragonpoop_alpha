
#ifndef dragonpoop_openglx_1o5_renderer_factory_h
#define dragonpoop_openglx_1o5_renderer_factory_h

#include "../renderer_factory.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_factory : public renderer_factory
    {
        
    private:
        
    protected:
        
        //generate renderer
        virtual renderer *genRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        //test renderer
        virtual bool testRenderer( core *c, gfx_writelock *g, dptaskpool_writelock *tp );
        
    public:
        
        //ctor
        openglx_1o5_renderer_factory( void );
        //dtor
        virtual ~openglx_1o5_renderer_factory( void );
        
    };
    
};

#endif