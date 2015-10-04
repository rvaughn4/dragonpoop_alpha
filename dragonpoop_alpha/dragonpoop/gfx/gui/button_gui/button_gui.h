
#ifndef dragonpoop_button_gui_h
#define dragonpoop_button_gui_h

#include "../gui.h"

namespace dragonpoop
{
    
    class button_gui : public gui
    {
        
    private:
        
    protected:
        
        //override to paint background texture
        virtual void repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h );
        
    public:
        
        //ctor
        button_gui( gfx_writelock *g, dpid id, dpid pid, float x, float y, float w, float h, const char *txt );
        //dtor
        virtual ~button_gui( void );
        
    };
    
};

#endif