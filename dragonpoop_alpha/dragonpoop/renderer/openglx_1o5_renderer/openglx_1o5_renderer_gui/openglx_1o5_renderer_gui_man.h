
#ifndef dragonpoop_openglx_1o5_renderer_gui_man_h
#define dragonpoop_openglx_1o5_renderer_gui_man_h

#include "../../renderer_gui/renderer_gui_man.h"
#include "../openglx_1o5_renderer.h"

namespace dragonpoop
{
    
    class openglx_1o5_renderer_gui_man : public renderer_gui_man
    {
        
    private:
        
        openglx_1o5_renderer *r;
        
    protected:
        
        //generate renderer gui
        virtual renderer_gui *genGui( gui_writelock *ml );
        
    public:
        
        //ctor
        openglx_1o5_renderer_gui_man( core *c, openglx_1o5_renderer *r, dptaskpool_writelock *tp );
        //dtor
        virtual ~openglx_1o5_renderer_gui_man( void );
        
    };
    
};

#endif