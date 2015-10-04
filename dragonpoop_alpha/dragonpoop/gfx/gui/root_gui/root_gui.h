
#ifndef dragonpoop_root_gui_h
#define dragonpoop_root_gui_h

#include "../gui.h"

namespace dragonpoop
{
    
    class root_gui : public gui
    {
        
    private:
        
        gui *b;
        
    protected:
        
    public:
        
        //ctor
        root_gui( gfx_writelock *g, dpid id );
        //dtor
        virtual ~root_gui( void );
        
    };
    
};

#endif