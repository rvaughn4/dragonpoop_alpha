
#ifndef dragonpoop_openglx_1o5_renderer_model_writelock_h
#define dragonpoop_openglx_1o5_renderer_model_writelock_h

#include "../../renderer_model/renderer_model_writelock.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model;
    
    class openglx_1o5_renderer_model_writelock : public renderer_model_writelock
    {
        
    private:
        
        openglx_1o5_renderer_model *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_model_writelock( openglx_1o5_renderer_model *t, dpmutex_writelock *l );
        //dtor
        virtual ~openglx_1o5_renderer_model_writelock( void );
        
    public:
        
        friend class openglx_1o5_renderer_model;
    };
    
};

#endif