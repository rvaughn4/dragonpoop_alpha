
#ifndef dragonpoop_openglx_1o5_renderer_gui_readlock_h
#define dragonpoop_openglx_1o5_renderer_gui_readlock_h

#include "../../renderer_gui/renderer_gui_readlock.h"

namespace dragonpoop
{
    class openglx_1o5_renderer_gui;

    class openglx_1o5_renderer_gui_readlock : public renderer_gui_readlock
    {
        
    private:
        
        openglx_1o5_renderer_gui *t;
        
    protected:
        
        //ctor
        openglx_1o5_renderer_gui_readlock( openglx_1o5_renderer_gui *t, dpmutex_readlock *l );
        //dtor
        virtual ~openglx_1o5_renderer_gui_readlock( void );
        
    public:
        
        friend class openglx_1o5_renderer_gui;
    };
    
};

#endif