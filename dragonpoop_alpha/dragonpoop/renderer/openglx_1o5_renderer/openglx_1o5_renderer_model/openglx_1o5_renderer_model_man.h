
#ifndef dragonpoop_openglx_1o5_renderer_model_man_h
#define dragonpoop_openglx_1o5_renderer_model_man_h

#include "../../renderer_model/renderer_model_man.h"
#include "../openglx_1o5_renderer.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model_man : public renderer_model_man
    {
        
    private:
        
        openglx_1o5_renderer *r;
        
    protected:
        
        //generate renderer model
        virtual renderer_model *genModel( model_writelock *ml );
        
    public:
        
        //ctor
        openglx_1o5_renderer_model_man( core *c, openglx_1o5_renderer *r, dptaskpool_writelock *tp );
        //dtor
        virtual ~openglx_1o5_renderer_model_man( void );
        
    };
    
};

#endif