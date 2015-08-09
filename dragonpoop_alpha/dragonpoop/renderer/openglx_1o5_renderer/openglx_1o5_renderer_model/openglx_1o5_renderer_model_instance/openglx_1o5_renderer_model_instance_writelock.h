
#ifndef dragonpoop_openglx_1o5_renderer_model_instance_writelock_h
#define dragonpoop_openglx_1o5_renderer_model_instance_writelock_h

#include "../../../renderer_model/renderer_model_instance/renderer_model_instance_writelock.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model_instance;
    
    class openglx_1o5_renderer_model_instance_writelock : public renderer_model_instance_writelock
    {
        
    private:
        
        openglx_1o5_renderer_model_instance *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_model_instance_writelock( openglx_1o5_renderer_model_instance *t, dpmutex_writelock *l );
        //dtor
        virtual ~openglx_1o5_renderer_model_instance_writelock( void );
        
    public:
        
        friend class openglx_1o5_renderer_model_instance;
    };
    
};

#endif