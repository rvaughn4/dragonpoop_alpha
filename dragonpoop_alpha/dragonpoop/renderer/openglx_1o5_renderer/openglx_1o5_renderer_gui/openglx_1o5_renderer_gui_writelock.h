
#ifndef dragonpoop_openglx_1o5_renderer_gui_writelock_h
#define dragonpoop_openglx_1o5_renderer_gui_writelock_h

#include "../../renderer_gui/renderer_gui_writelock.h"

namespace dragonpoop
{
    class openglx_1o5_renderer_gui;
    
    class openglx_1o5_renderer_gui_writelock : public renderer_gui_writelock
    {
        
    private:
        
        openglx_1o5_renderer_gui *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_gui_writelock( openglx_1o5_renderer_gui *t, dpmutex_writelock *l );
        //dtor
        virtual ~openglx_1o5_renderer_gui_writelock( void );
        
    public:
        
        friend class openglx_1o5_renderer_gui;
    };
    
};

#endif