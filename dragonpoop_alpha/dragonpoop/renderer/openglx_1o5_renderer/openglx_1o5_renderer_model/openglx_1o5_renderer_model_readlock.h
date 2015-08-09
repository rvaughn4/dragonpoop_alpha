
#ifndef dragonpoop_openglx_1o5_renderer_model_readlock_h
#define dragonpoop_openglx_1o5_renderer_model_readlock_h

#include "../../renderer_model/renderer_model_readlock.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model;
    
    class openglx_1o5_renderer_model_readlock : public renderer_model_readlock
    {
        
    private:
        
        openglx_1o5_renderer_model *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_model_readlock( openglx_1o5_renderer_model *t, dpmutex_readlock *l );
        //dtor
        virtual ~openglx_1o5_renderer_model_readlock( void );
        
    public:
        
        friend class openglx_1o5_renderer_model;
    };
    
};

#endif