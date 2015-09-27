
#ifndef dragonpoop_openglx_1o5_renderer_gui_ref_h
#define dragonpoop_openglx_1o5_renderer_gui_ref_h

#include "../../renderer_gui/renderer_gui_ref.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_gui;
    
    class openglx_1o5_renderer_gui_ref : public renderer_gui_ref
    {
        
    private:
        
        openglx_1o5_renderer_gui *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_gui_ref( openglx_1o5_renderer_gui *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //dtor
        virtual ~openglx_1o5_renderer_gui_ref( void );
        
        friend class openglx_1o5_renderer_gui;
    };
    
};

#endif