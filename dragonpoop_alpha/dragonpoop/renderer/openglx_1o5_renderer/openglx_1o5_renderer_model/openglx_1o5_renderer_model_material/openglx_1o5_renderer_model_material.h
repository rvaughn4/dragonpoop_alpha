
#ifndef dragonpoop_openglx_1o5_renderer_model_material_h
#define dragonpoop_openglx_1o5_renderer_model_material_h

#include "../../../renderer_model/renderer_model_material/renderer_model_material.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_model_material : public renderer_model_material
    {
        
    private:
        
        
    protected:
        
    public:
        
        //ctor
        openglx_1o5_renderer_model_material( model_writelock *ml, model_material *m );
        //dtor
        virtual ~openglx_1o5_renderer_model_material( void );
        //sync with group
        virtual void sync( model_writelock *ml, model_material *m );
        
    };
    
};

#endif